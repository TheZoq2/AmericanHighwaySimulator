#include "car.hpp"

#include "consts.hpp"

Car::Car(sf::Vector2f position) : Car(VehicleType::CAR, position){}
Car::Car(VehicleType type, sf::Vector2f position) {
    this->type = type;
    this->position = position;

    // TODO FIXME TODO FIXME TODO FIXME TODO FIXME TODO FIXME TODO FIXME 
    // Change these
    // TODO FIXME TODO FIXME TODO FIXME TODO FIXME TODO FIXME TODO FIXME 
    this->width = PLAYER_WIDTH;
    this->height = PLAYER_HEIGHT;
    this->wrecked = false;

    this->velocity = CAR_SPEED + (random() % CAR_SPEED_VARIATION);
}

void Car::draw(sf::RenderTarget* target, Assets& assets) const {
    assets.generic_car.draw(target, this->position);
}


void Car::update(float delta) {
    if (!this->wrecked) {
        this->position.y += delta * velocity;
    } else {
        this->position.y += delta * ROAD_SPEED;
    }
}
