#include "level.hpp"
#include <iostream>

#include <algorithm>
#include <cmath>

Level::Level(int num_lanes) {
    this->num_lanes = num_lanes;

    this->road_width = lane_amount * LANE_WIDTH;

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


void Level::update(float delta_time) {
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
        return car.position.y < WINDOW_HEIGHT * 2;
    });

    this->cars = new_cars;

    // Spawn new cars
    while(cars.size() < CAR_AMOUNT) {
        spawn_car();
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
        on_player_collision_with_car(collision.p, collision.car);
    }

    update_players_handle_input(delta_time);
    update_and_spawn_powerups(delta_time);
}

void Level::update_players_handle_input(float delta_time) {
    for (auto& player : players) {
        if (player.powerup != nullptr) {
            player.powerup->angle += POWERUP_ANGLE_SPEED*delta_time;
        }

        // wrecked cars can't move
        if (player.wrecked) {
            player.position.y += ROAD_SPEED*delta_time;
            continue;
        }

        float y_retardation = 1.;
        if (is_offroad(player.position, PLAYER_WIDTH)) {
            y_retardation = PLAYER_OFFROAD_ACC_RETARDATION;
            player.position.y += PLAYER_OFFROAD_VEL_RETARDATION*delta_time;
        }

        int dx{0}, dy{0};
        dy += PLAYER_ACCELERATION_Y_DOWN
            * player.input_handler->get_value(input::Action::DOWN)
            * (1/y_retardation);
        dy -= PLAYER_ACCELERATION_Y
            * player.input_handler->get_value(input::Action::UP)
            * y_retardation;
        dx -= PLAYER_ACCELERATION_X
            * player.input_handler->get_value(input::Action::LEFT);
        dx += PLAYER_ACCELERATION_X
            * player.input_handler->get_value(input::Action::RIGHT);

        sf::Vector2f acceleration(dx, dy);
        acceleration += player.persistent_acceleration;
        acceleration *= delta_time;

        player.velocity += acceleration;

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


void Level::spawn_car() {
    auto lane = random() % lane_amount;
    // +0.5 to put the car in the center of the lane rather than on the side
    auto position = WINDOW_CENTER - road_width / 2 + LANE_WIDTH * (lane + 0.5);

    auto spawn_offset = random() % CAR_SPAWN_MAX_OFFSET;

    this->cars.push_back(Car(sf::Vector2f(position, CAR_SPAWN_Y - spawn_offset)));
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
    collider->velocity.x = sign * PLAYER_MAX_VEL_X * 0.1 - avg_velocity;
    collided->velocity.x = -sign * PLAYER_MAX_VEL_X * 0.1 + avg_velocity;

    std::cout << collider->name << " collided with " 
        << collided->name << "!" << std::endl;
}

void Level::on_player_collision_with_car(Player* p, Car* c) {
    p->position -= p->velocity;
    p->persistent_acceleration.x +=
        (random() * COLLISION_MAX_BREAKAGE) - COLLISION_MAX_BREAKAGE / 2;
    p->health -= COLLISION_DAMAGE;

    if(p->health < 0) {
        // p->wrecked = true;
    }
    // p->wrecked = true;
    c->wrecked = true;

    float sign = -1;
    if(p->position.x > c->position.x) {
        sign = 1;
    }
    p->velocity.x = sign * PLAYER_MAX_VEL_X * 0.1;
}

CarCollisionResult Level::check_car_collisions() {

    for (auto& player : players) {
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

    PowerUpType type = static_cast<PowerUpType>(random() % NUM_POWERUPS);
    this->powerups.push_back(
            PowerUp {sf::Vector2f(position, CAR_SPAWN_Y - spawn_offset), 
            type, 0});
}

void Level::update_and_spawn_powerups(float delta_time) {
    // maybe spawn powerup
    auto r = random() % (int)(1/POWERUP_SPAWN_PROBABILITY);
    if (r == 0) {
        spawn_powerup();
        std::cout << "SPAWNED POWERUP" << std::endl;
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
            if (powerup_collides_with_player(powerup, &player)) {
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

void Level::activate_sleepy_powerup() {
    std::cout << "Sleepy!" << std::endl;
}

void Level::activate_transparency_powerup() {
    std::cout << "Transparency!" << std::endl;
}

