#include "level.hpp"
#include <iostream>

#include <algorithm>
#include <cmath>

Level::Level(int num_lanes, Assets& assets) {
    this->num_lanes = num_lanes;

    this->road_width = lane_amount * LANE_WIDTH;

    sf::Texture bg_texture;
    bg_texture.loadFromFile("../resources/grass.png");

    sf::Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);

    this->police = assets.siren.get_sound();
    this->macke = assets.macke.get_sound();

}

Level::~Level() { }

void Level::draw(sf::RenderTarget* target, Assets& assets) const {
    for (auto& lane : lanes) {
        lane.draw(target, assets);
    }

    for (auto& car: cars) {
        car.draw(target, assets);
    }

    for (auto& player: players) {
        player.draw(target, assets);
    }

    for (auto& powerup : powerups) {
        powerup.draw(target, assets);
    }
}


void Level::update(float delta_time, Assets& assets) {
    if(get_players_left() < 1) {
        std::cout << "No players left " << game_over_timeout << std::endl;
        game_over_timeout -= delta_time;
    }

    check_if_players_within_bounds();
    // Update lanes
    for (auto& lane : lanes) {
        lane.update(delta_time);
    }

    // Update cars
    for(auto& car: cars) {
        car.update(delta_time);
    }

    // Filter removed cars
    std::vector<Car> new_cars;
    std::copy_if(cars.begin(), cars.end(), std::back_inserter(new_cars), [&](auto car) {
        if(car.type == VehicleType::POLICE) {
            return car.position.y > -500 && car.position.y < WINDOW_HEIGHT * 3;
        }
        else {
            return car.position.y < WINDOW_HEIGHT * 2;
        }
    });

    this->cars = new_cars;

    // Spawn new cars
    while(cars.size() < CAR_AMOUNT) {
        spawn_car(assets);
    }

    // Filter removed lanes
    std::vector<Lane> new_lanes;
    std::copy_if(lanes.begin(), lanes.end(), std::back_inserter(new_lanes), [&](auto lane) {
        return lane.position.y < WINDOW_HEIGHT;
    });
    this->lanes = new_lanes;

    // Add new lanes
    while (lanes.size() < (size_t)lane_amount * 2) {
        for (int i = 0; i < lane_amount; i++) {
            add_lane(i);
        }
    }

    CarCollisionResult collision = check_car_collisions();
    if (collision.collision_occurred) {
        on_player_collision_with_car(collision.p, collision.car, assets);
    }

    car_on_car_collision();

    update_and_spawn_powerups(delta_time);
    update_players_handle_input(delta_time);
}

void Level::update_players_handle_input(float delta_time) {
    for (auto& player : players) {
        player.update_engine_noise();
        update_target_selection(&player, delta_time);
        player.shake_left -= delta_time;

        if (player.powerup != nullptr) {
            player.powerup->angle += POWERUP_ANGLE_SPEED*delta_time;
        }

        // wrecked cars can't move
        if (player.wrecked) {
            player.velocity.x -= (
                player.velocity.x * WRECK_RETARDATION * delta_time
            );
            player.velocity.y -= (
                (player.velocity.y - ROAD_SPEED) * WRECK_RETARDATION * delta_time
            );
            player.position += player.velocity * delta_time;
            // player.position.y += ROAD_SPEED*delta_time;
            continue;
        }

        // handle transparency
        if (player.is_transparent()) {
            player.transparency_time -= delta_time;
        }
        player.bmv_time -= delta_time;

        int dx{0}, dy{0};
        dy += PLAYER_ACCELERATION_Y_DOWN
            * player.input_handler->get_value(input::Action::DOWN);
        dy -= PLAYER_ACCELERATION_Y
            * player.input_handler->get_value(input::Action::UP);
        dx -= PLAYER_ACCELERATION_X
            * player.input_handler->get_value(input::Action::LEFT);
        dx += PLAYER_ACCELERATION_X
            * player.input_handler->get_value(input::Action::RIGHT);


        auto sign = 1;
        if (player.is_inverted()) {
            sign = -1;
        }

        auto extra_performance = 1.0;
        if(player.bmv_time > 0) {
            extra_performance = 1.7;
        }

        auto target_y_velocity =
            ( player.input_handler->get_value(input::Action::DOWN)
            - player.input_handler->get_value(input::Action::UP)
            )
            * PLAYER_MAX_VEL_Y
            * sign
            * extra_performance;
        auto target_x_velocity =
            ( player.input_handler->get_value(input::Action::RIGHT)
            - player.input_handler->get_value(input::Action::LEFT)
            )
            * PLAYER_MAX_VEL_X
            * sign
            * extra_performance
            + player.persistent_acceleration.x;

        if (is_offroad(player.position, PLAYER_WIDTH)) {
            target_y_velocity += PLAYER_MAX_VEL_Y / 2;
        }

        if (!player.is_sleepy()) {
            // player.velocity.x += acceleration.x;
            player.velocity.y += (target_y_velocity - player.velocity.y) * 0.03 * extra_performance;
            player.velocity.x += (target_x_velocity - player.velocity.x) * 0.04 *extra_performance;
        }
        else {
            target_y_velocity = 0;
            player.velocity.y += (target_y_velocity - player.velocity.y) * 0.03;
        }

        update_sleepiness(&player, delta_time);
        update_invertedness(&player, delta_time);

        if (player.velocity.x > PLAYER_MAX_VEL_X) {
            player.velocity.x = PLAYER_MAX_VEL_X;
        }
        if (-player.velocity.x < -PLAYER_MAX_VEL_X) {
            player.velocity.x = PLAYER_MAX_VEL_X;
        }

        sf::Vector2f new_pos = player.position + player.velocity * delta_time;
        Player* collided = get_colliding_player(&player, new_pos);
        if (collided == nullptr) {
            player.position += player.velocity * delta_time;
        } else {
            // this is to prevent on_player_collision to be fired
            // more than once per collision
            if (player.just_collided_with != collided) {
                on_player_collision_with_other(&player, collided);
            }
        }
        // this is to prevent on_player_collision to be fired
        // more than once per collision
        player.just_collided_with = collided;

        if (player.input_handler->get_value(input::Action::FIRE) &&
                !player.is_sleepy() &&
                !player.is_bmv() &&
                !player.is_transparent() &&
            (player.powerup != nullptr || player.selection_mode)) {
            if (!player.already_entered_selection) {
                fire_player_powerup(&player);
            }
            player.already_entered_selection = true;
        } else if (player.already_entered_selection) {
            player.already_entered_selection = false;
        }
    }
}

Player* Level::get_colliding_player(const Player* p, sf::Vector2f new_pos) {
    for (size_t i{0}; i < players.size(); ++i) {
        Player* other = &players[i];

        // we don't care if we collide with ourselves
        if (other != p) {
            sf::Vector2f pos = other->position;

            float new_x = new_pos.x;
            float other_x = pos.x;
            float new_y = new_pos.y;
            float other_y = pos.y;

            if (std::abs(new_x - other_x) < PLAYER_WIDTH &&
                std::abs(new_y - other_y) < PLAYER_HEIGHT) {
                return other;
            }
        }
    }
    return nullptr;
}

void Level::add_player(Player& player) {
    players.push_back(player);
}


////////////////////////////////////////////////////////////////////////////////
//          Private members
////////////////////////////////////////////////////////////////////////////////


void Level::spawn_car(Assets& assets) {
    auto spawn_offset = random() % CAR_SPAWN_MAX_OFFSET;
    auto seed = random() % 100;
    if(seed > 40) {
        auto type = VehicleType::CAR;
        if(seed > 90) {
            type = VehicleType::MOTORBIKE;
        }
        else if(seed > 80) {
            type = VehicleType::TRUCK;
        }
        else if(seed > 70) {
            type = VehicleType::TRACTOR;
        }
        auto lane = random() % lane_amount;
        // +0.5 to put the car in the center of the lane rather than on the side
        auto position = WINDOW_CENTER - road_width / 2 + LANE_WIDTH * (lane + 0.5);

        this->cars.push_back(
            Car(type, sf::Vector2f(position, CAR_SPAWN_Y - spawn_offset), assets)
        );
    }
    else {
        auto range = (WINDOW_WIDTH - road_width);
        auto position = (random() % range) - range/2;

        if(position > 0) {
            position += road_width / 2 + ROCK_SIZE / 2;
        }
        else {
            position -= road_width / 2 + ROCK_SIZE / 2;
        }

        position += WINDOW_CENTER;

        this->cars.push_back(Car(
            VehicleType::ROCK,
            sf::Vector2f(position, CAR_SPAWN_Y - spawn_offset), assets
        ));
    }
}

void Level::add_lane(int lane_num) {
    auto x_position = WINDOW_CENTER - (LANE_WIDTH*lane_amount/2) + LANE_WIDTH * lane_num;

    this->lanes.push_back(Lane(sf::Vector2f(x_position, 0)));
}

void Level::on_player_collision_with_other(Player* collider, Player* collided) {
    if (collided->wrecked) {
        collider->wrecked = true;
    }
    float avg_velocity = (collider->velocity.x - collider->velocity.x) / 2;
    float sign = -1;
    if(collider->position.x > collided->position.x) {
        sign = 1;
    }
    collider->velocity.x = sign * PLAYER_MAX_VEL_X * 1 - avg_velocity;
    collided->velocity.x = -sign * PLAYER_MAX_VEL_X * 1 + avg_velocity;

}

void Level::on_player_collision_with_car(Player* p, Car* c, Assets& assets) {
    // Shake the player for a short time
    if(c->type == VehicleType::ROCK) {
        p->wrecked = true;
    }
    if(p->collidee != c) {
        p->collidee = c;
        p->persistent_acceleration.x +=
            (random() % COLLISION_MAX_BREAKAGE) - COLLISION_MAX_BREAKAGE / 2;

        if(c->type != VehicleType::MOTORBIKE) {
            // Update health
            p->health -= COLLISION_DAMAGE;

            // Bounce
            float sign = -1;
            if(p->position.x > c->position.x) {
                sign = 1;
            }
            p->velocity.x = sign * PLAYER_MAX_VEL_X * 0.1;
        }
        else {
            // shake the car a bit since you ran over someone
            p->shake_left = 0.25;
        }

        if(p->health < 0) {
            p->wrecked = true;
        }
        // p->wrecked = true;
        if(c->type != VehicleType::TRUCK) {
            c->wrecked = true;
        }

        if(c->type == VehicleType::MOTORBIKE) {
            sf::Vector2f position(c->position.x, POLICE_SPAWN_DISTANCE);
            this->cars.push_back(
                Car(VehicleType::POLICE, position, assets)
            );
            this->macke->play();
        }
    }
}

CarCollisionResult Level::check_car_collisions() {
    for (auto& player : players) {

        if (player.is_transparent()) {
            continue;
        }

        for (auto& car : cars) {
            float px = player.position.x;
            float py = player.position.y;
            float cx = car.position.x;
            float cy = car.position.y;
            float cw = car.width;
            float ch = car.height;

            if (2*std::abs(px - cx) < cw + PLAYER_WIDTH &&
                2*std::abs(py - cy) < ch + PLAYER_HEIGHT &&
                !player.wrecked) {
                return CarCollisionResult{true, &player, &car};
            }
        }

        player.collidee = nullptr;
    }
    return {false, nullptr, nullptr};
}



bool Level::is_offroad(sf::Vector2f pos, int width) const {
    int left_edge = WINDOW_CENTER - road_width/2;
    int right_edge = WINDOW_CENTER + road_width/2;
    return pos.x <= left_edge || pos.x + width >= right_edge;
}

void Level::check_if_players_within_bounds() {
    for (auto& player : players) {
        if (player.position.x + PLAYER_WIDTH < 0 ||
            player.position.x > WINDOW_WIDTH ||
            player.position.y + PLAYER_HEIGHT < 0 ||
            player.position.y > WINDOW_HEIGHT) {
            player.wrecked = true;
        }
    }
}


void Level::spawn_powerup() {
    auto lane = random() % lane_amount;
    // +0.5 to put the car in the center of the lane rather than on the side
    auto position = WINDOW_CENTER - road_width / 2 + LANE_WIDTH * (lane + 0.5);

    auto spawn_offset = random() % CAR_SPAWN_MAX_OFFSET;

    PowerUpType type;

    do {
        type = static_cast<PowerUpType>(random() % NUM_POWERUPS);

    // only spawn non-attack powerups if only one person is playing
    } while((type == PowerUpType::SLEEPY || type == PowerUpType::INVERTED)
                && players.size() == 1);

    this->powerups.push_back(
            PowerUp {sf::Vector2f(position, CAR_SPAWN_Y - spawn_offset), 
            type, 0});
}

void Level::update_and_spawn_powerups(float delta_time) {
    // maybe spawn powerup
    auto r = random() % (int)(1/POWERUP_SPAWN_PROBABILITY);
    if (r == 0) {
        spawn_powerup();
    }

    // update powerups
    for (auto& powerup : powerups) {
        powerup.position.y += ROAD_SPEED*delta_time;
        powerup.angle += POWERUP_ANGLE_SPEED*delta_time;
    }
    
    // check for player collisions
    std::vector<size_t> indices_to_remove;
    for (size_t i{0}; i < powerups.size(); ++i) {
        PowerUp* powerup = &powerups[i];
        for (auto& player : players) {
            if (powerup_collides_with_player(powerup, &player) &&
                !player.selection_mode &&
                !player.is_sleepy()) {
                // copy the powerup to the player
                PowerUp* p = new PowerUp(*powerup);
                player.set_powerup(p);
                indices_to_remove.push_back(i);
            }
        }
    }
    // remove the picked-up powerups
    for (auto i : indices_to_remove) {
        powerups.erase(powerups.begin() + i);
    }
}

bool Level::powerup_collides_with_player(PowerUp* pu, Player* p) const {
    return std::abs(p->position.x - pu->position.x) < POWERUP_WIDTH &&
           std::abs(p->position.y - pu->position.y) < POWERUP_HEIGHT;
}

void Level::fire_player_powerup(Player* p) {
    if (p->powerup == nullptr) {
        if (p->next_powerup == PowerUpType::INVERTED) {
            activate_inverted_powerup(p);
        } else if (p->next_powerup == PowerUpType::SLEEPY) {
            activate_sleepy_powerup(p);
        }
        return;
    }
    switch (p->powerup->type) {
        case PowerUpType::SLEEPY:
            activate_sleepy_powerup(p);
            break;
        case PowerUpType::TRANSPARENCY:
            activate_transparency_powerup(p);
            break;
        case PowerUpType::BMV:
            activate_bmv_powerup(p);
            break;
        case PowerUpType::INVERTED:
            activate_inverted_powerup(p);
            break;
    }
    delete p->powerup;
    p->powerup = nullptr;
}

void Level::actually_fire_sleepy_powerup(Player* p) {
    size_t index = p->selected_target_index;
    Player* target = &players[index];
    target->sleepy_time = SLEEPY_TIME;
}

void Level::actually_fire_inverted_powerup(Player* p) {
    size_t index = p->selected_target_index;
    Player* target = &players[index];
    target->inverted_time = INVERTED_TIME;
}

void Level::activate_sleepy_powerup(Player* p) {
    p->next_powerup = PowerUpType::SLEEPY;
    if (p->selection_mode) {
        std::cout << "Sleepy!" << std::endl;
        deactivate_target_selection(p);
        actually_fire_sleepy_powerup(p);
    } else {
        if (!this->someone_selecting) {
            activate_target_selection(p);
        }
    }
}

void Level::update_sleepiness(Player* p, float delta_time) {
    if (p->is_sleepy()) {
        p->sleepy_time -= delta_time;
    }
}

void Level::update_invertedness(Player* p, float delta_time) {
    if (p->is_inverted()) {
        p->inverted_time -= delta_time;
    }
}

void Level::activate_transparency_powerup(Player* p) {
    p->transparency_time = TRANSPARENCY_TIME;
    std::cout << "Transparency!" << std::endl;
}

void Level::activate_bmv_powerup(Player* p) {
    p->bmv_time = BMV_TIME;
    std::cout << "Brutto Mational Value!" << std::endl;
}

void Level::activate_inverted_powerup(Player* p) {
    p->next_powerup = PowerUpType::INVERTED;
    if (p->selection_mode) {
        std::cout << "INVERTED!" << std::endl;
        deactivate_target_selection(p);
        actually_fire_inverted_powerup(p);
    } else {
        if (!this->someone_selecting) {
            activate_target_selection(p);
        }
    }
}

void Level::activate_target_selection(Player* p) {
    this->someone_selecting = true;
    size_t initial_index = 0; 
    if (&players[0] == p) {
        initial_index++;
    }
    p->selected_target_index = initial_index;
    p->selection_mode = true;
    p->selection_time = TARGET_SELECTION_TIME;
    players[initial_index].target_selected = true;
    players[initial_index].selected_by = p;
}

void Level::deactivate_target_selection(Player* p) {
    this->someone_selecting = false;
    p->selection_mode = false;
    p->selection_time = 0;
    players[p->selected_target_index].target_selected = false;
    players[p->selected_target_index].selected_by = nullptr;
}

void Level::update_target_selection(Player* p, float delta_time) {
    if (p->selection_mode) {
        if (p->selection_time > 0) {
            p->selection_time -= delta_time;
        } else {
            std::cout << "changing" << std::endl;
            // increment selection
            players[p->selected_target_index].target_selected = false;
            players[p->selected_target_index].selected_by = nullptr;
            p->selection_time = TARGET_SELECTION_TIME;
            size_t index = (p->selected_target_index + 1) % players.size();
            if (&players[index] == p) {
                index = (index + 1) % players.size();
            }
            p->selected_target_index = index;
            players[index].target_selected = true;
            players[index].selected_by = p;
        }
    }
}


void Level::car_on_car_collision() {
    /*
    for(auto& car : cars) {
        for(auto& other: cars) {
            // If these are the same cars or they are not in the same lane
            auto distance = car.position.y - other.position.y;

            if( car.position.y == other.position.y 
                || car.position.x != other.position.x
                || distance > 0
                // || car.wrecked
              )
            {
                break;
            }

            if(distance < (car.height + other.height)) {
                // This one is catching up, slow down
                car.velocity = other.velocity;
            }
        }
    }
    */
}

int Level::get_players_left() {
    int cars_alive = 0;
    for (auto& player : players) {
        if (!player.wrecked) {
            cars_alive++;
        }
    }
    return cars_alive;
}

void Level::reset_players() {
    players.clear();
}

void Level::reset_cars() {
    cars.clear();
}
