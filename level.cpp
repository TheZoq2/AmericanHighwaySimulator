#include "level.hpp"

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
}

