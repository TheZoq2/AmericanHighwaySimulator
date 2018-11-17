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

    if(type == VehicleType::ROCK) {
        this->velocity = ROAD_SPEED;
    }
    else {
        this->velocity = CAR_SPEED + (random() % CAR_SPEED_VARIATION);
    }
}

void Car::draw(sf::RenderTarget* target, Assets& assets) const {
    if(type == VehicleType::ROCK) {
        assets.rock.draw(target, this->position);
    }
    else {
        auto asset = assets.generic_car[0];
        if(wrecked) {
            asset = assets.generic_car[3];
        }
        asset.draw(target, this->position);
    }
}


void Car::update(float delta) {
    if (!this->wrecked) {
        this->position.y += delta * velocity;
    } else {
        this->position.y += delta * ROAD_SPEED;
    }
}
