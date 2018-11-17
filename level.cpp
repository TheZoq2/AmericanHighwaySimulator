#include "level.hpp"
#include <iostream>

#include "consts.hpp"

#include <algorithm>

Level::Level(int num_lanes) {
    this->num_lanes = num_lanes;

    this->cars.push_back(Car(VehicleType::CAR, sf::Vector2f(100, 100)));

    this->road_width = lane_amount * LANE_WIDTH;
}

Level::~Level() { }


void Level::draw(sf::RenderTarget* target, Assets& assets) const {
    for(auto car: cars) {
        car.draw(target, assets);
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
}

void Level::handle_input() {
    for (auto& player : players) {
        int dx{0}, dy{0};
        if (player.is_pressed(input::Action::DOWN)) {
            dy -= PLAYER_SPEED;
        } else if (player.is_pressed(input::Action::UP)) {
            dy += PLAYER_SPEED;
        }
        if (player.is_pressed(input::Action::LEFT)) {
            dx += PLAYER_SPEED;
        } else if (player.is_pressed(input::Action::RIGHT)) {
            dx -= PLAYER_SPEED;
        }

        sf::Vector2f dxdy(dx, dy);
        player.position += dxdy;
    }
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
