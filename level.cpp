#include "level.hpp"
#include <iostream>

#include "consts.hpp"

#include <algorithm>

Level::Level(int num_lanes) {
    this->num_lanes = num_lanes;

    this->cars.push_back(Car(VehicleType::CAR, sf::Vector2f(100, 100)));
}

Level::~Level() { }


void Level::draw(sf::RenderTarget* target, Assets& assets) const {
    for(auto car: cars) {
        car.draw(target, assets);
    }
}


void Level::update() {
    for(auto& car: cars) {
        car.update();
    }

    std::vector<Car> new_cars;
    std::copy_if(cars.begin(), cars.end(), std::back_inserter(new_cars), [&](auto car) {
        return car.position.y < WINDOW_HEIGHT * 2;
    });

    this->cars = new_cars;
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

