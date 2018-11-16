#include "level.hpp"

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
}

