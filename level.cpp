#include "level.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include "consts.hpp"

Level::Level(int num_lanes) {
    this->num_lanes = num_lanes;

    this->cars.push_back(Car(VehicleType::CAR, sf::Vector2f(100, 100)));

    this->road_width = lane_amount * LANE_WIDTH;
}

Level::~Level() { }


void Level::draw(sf::RenderTarget* target, Assets& assets) const {
    for (auto& car: cars) {
        car.draw(target, assets);
    }

    for (auto& player: players) {
        player.draw(target, assets);
    }
}


void Level::update(float delta_time) {
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
    if(cars.size() < CAR_AMOUNT) {
        if(random() % 100 > CAR_SPAWN_PROBABILITY ) {
            spawn_car();
        }
    }


    handle_input();

    for (auto& player : players) {
        std::cout << player.name << ": " <<
            player.position.x << ", " << player.position.y << std::endl;
    }

    CarCollisionResult collision = check_car_collisions();
    if (collision.collision_occurred) {
        on_player_collision_with_car(collision.p, collision.car);
    }
}

void Level::handle_input() {
    for (auto& player : players) {
        int dx{0}, dy{0};
        if (player.is_pressed(input::Action::DOWN)) {
            dy += PLAYER_SPEED;
        }
        if (player.is_pressed(input::Action::UP)) {
            dy -= PLAYER_SPEED;
        }
        if (player.is_pressed(input::Action::LEFT)) {
            dx -= PLAYER_SPEED;
        }
        if (player.is_pressed(input::Action::RIGHT)) {
            dx += PLAYER_SPEED;
        }

        sf::Vector2f dxdy(dx, dy);
        sf::Vector2f new_pos = player.position + dxdy;
        Player* collided = get_colliding_player(&player, new_pos);
        if (collided == nullptr) {
            player.position += dxdy;
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

    this->cars.push_back(Car(sf::Vector2f(position, CAR_SPAWN_Y)));
}

void Level::on_player_collision_with_other(Player* collider, Player* collided) {
    // TODO do something fun
    std::cout << collider->name << " collided with " 
        << collided->name << "!" << std::endl;
}

void Level::on_player_collision_with_car(const Player* p, const Car* c) {
    // TODO do something fun
    std::cout << p->name << " collided with a car!" << std::endl;
}

CarCollisionResult Level::check_car_collisions() const {

    for (auto& player : players) {
        for (auto& car : cars) {
            float px = player.position.x;
            float py = player.position.y;
            float cx = car.position.x;
            float cy = car.position.y;
            float cw = car.width;
            float ch = car.height;

            if (2*std::abs(px - cx) < cw + PLAYER_WIDTH &&
                2*std::abs(py - cy) < ch + PLAYER_HEIGHT) {
                return {true, &player, &car};
            }
        }
    }
    return {false, nullptr, nullptr};
}

