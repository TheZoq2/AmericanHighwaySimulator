#include "car.hpp"

Car::Car(sf::Vector2f position) : Car(VehicleType::CAR, position){}
Car::Car(VehicleType type, sf::Vector2f position) {
    this->type = type;
    this->position = position;
}

void Car::draw(sf::RenderTarget* target, Assets& assets) const {
    assets.generic_car.draw(target, this->position);
}


void Car::update() {
    this->position.y += 0.5;
}
