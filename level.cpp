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
    while (lanes.size() < lane_amount * 2) {
        for (int i = 0; i < lane_amount; i++) {
            add_lane(i);
        }
    }

    CarCollisionResult collision = check_car_collisions();
    if (collision.collision_occurred) {
        on_player_collision_with_car(collision.p, collision.car);
    }

    update_players_handle_input(delta_time);
}

void Level::update_players_handle_input(float delta_time) {
    for (auto& player : players) {

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
        dy += PLAYER_ACCELERATION_Y
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
    p->wrecked = true;
    c->wrecked = true;

    std::cout << p->name << " collided with a car!" << std::endl;
}

sf::Vector2f rotate_vector(sf::Vector2f vec, float angle) {
    // SFML's angles are clockwise
    angle = -angle;
    sf::Vector2f(
        cos(angle) * vec.x - sin(angle) * vec.y,
        sin(angle) * vec.x + cos(angle) * vec.y
    );
}

CarCollisionResult Level::check_car_collisions() {
    std::vector<sf::Vector2f> points;

    for (auto& player : players) {
        if (player.wrecked) {
            continue;
        }

        for (auto& car : cars) {
            auto pos = player.position;
            float px = player.position.x;
            float py = player.position.y;

            float cx = car.position.x;
            float cy = car.position.y;
            float half_cw = car.width / 2;
            float half_ch = car.height / 2;

            float half_pw = PLAYER_WIDTH / 2;
            float half_ph = PLAYER_HEIGHT / 2;

            sf::Vector2f player_ur = pos + rotate_vector(sf::Vector2f(half_pw, -half_ph), player.angle);
            sf::Vector2f player_ul = pos + rotate_vector(sf::Vector2f(-half_pw, -half_ph), player.angle);
            sf::Vector2f player_lr = pos + rotate_vector(sf::Vector2f(half_pw, half_ph), player.angle);
            sf::Vector2f player_ll = pos + rotate_vector(sf::Vector2f(-half_pw, half_ph), player.angle);

            // NOTE: The following checks assume that the player will never rotate so much
            // that a point on the back of the car will be in front (more than 90 degrees)

            // Separation on y axis
            if (cy + half_ch < fmin(player_ur.y, player_ul.y) ||
                cy - half_ch > fmax(player_lr.y, player_ll.y)) {
                continue;
            }

            // Separation on x axis
            if (cx + half_cw < fmin(player_ul.x, player_ll.x) ||
                cx - half_cw > fmax(player_ur.x, player_lr.x)) {
                continue;
            }

            // axis1 and 2 are the "local y and x axis" of player
            sf::Vector2f axis1 = rotate_vector(sf::Vector2f(0, 1), player.angle);
            sf::Vector2f axis2 = rotate_vector(sf::Vector2f(1, 0), player.angle);

            float proj_pu = axis1.x * player_ur.x + axis1.y * player_ur.y;
            float proj_pl = axis1.x * player_ll.x + axis1.y * player_ll.y;
            float proj_cur = axis1.x * (cx + half_cw) + axis1.y * (cy - half_ch);
            float proj_cul = axis1.x * (cx - half_cw) + axis1.y * (cy - half_ch);
            float proj_clr = axis1.x * (cx + half_cw) + axis1.y * (cy + half_ch);
            float proj_cll = axis1.x * (cx - half_cw) + axis1.y * (cy + half_ch);

            // Separation on axis1
            if (proj_pu < fmin(proj_cur, proj_cul) ||
                proj_pl > fmax(proj_clr, proj_cll)) {
                continue;
            }

            proj_pu = axis2.x * player_ur.x + axis2.y * player_ur.y;
            proj_pl = axis2.x * player_ll.x + axis2.y * player_ll.y;
            proj_cur = axis2.x * (cx + half_cw) + axis2.y * (cy - half_ch);
            proj_cul = axis2.x * (cx - half_cw) + axis2.y * (cy - half_ch);
            proj_clr = axis2.x * (cx + half_cw) + axis2.y * (cy + half_ch);
            proj_cll = axis2.x * (cx - half_cw) + axis2.y * (cy + half_ch);

            // Separation on axis2
            if (proj_pu < fmin(proj_cul, proj_cll) ||
                proj_pl > fmax(proj_cur, proj_clr)) {
                continue;
            }

            // TODO: calculate intersecting points for sparks
            return CarCollisionResult{true, points, &player, &car};
        }
    }
    return {false, points, nullptr, nullptr};
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

