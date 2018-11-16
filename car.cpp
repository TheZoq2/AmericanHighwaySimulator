#include "car.hpp"

Car::Car(VehicleType type, sf::Vector2f position) {
    this->type = type;
    this->position = position;
}

void Car::draw(sf::RenderTarget* target, Assets& assets) const {
    assets.generic_car.draw(target, this->position);
}
