#include "car.hpp"

#include "consts.hpp"

Car::Car(sf::Vector2f position, Assets& assets) : 
    Car(VehicleType::CAR, position, assets){}
Car::Car(VehicleType type, sf::Vector2f position, Assets& assets) {
    this->type = type;
    this->position = position;

    if (type == VehicleType::POLICE) {
        this->sound = assets.siren.get_sound();
        this->sound->setVolume(90.f);
        this->sound->play();
    }

    this->width = PLAYER_WIDTH;
    this->height = PLAYER_HEIGHT;
    this->velocity = CAR_SPEED + (random() % CAR_SPEED_VARIATION);
    if(type == VehicleType::ROCK) {
        this->width = this->height = ROCK_SIZE;
        this->velocity = ROAD_SPEED;
    }
    if(type == VehicleType::MOTORBIKE) {
        this->width = MOTORCYCLE_WIDTH;
        this->height = MOTORCYCLE_HEIGHT;
        this->velocity *= 0.7;
    }
    if(type == VehicleType::TRUCK) {
        this->height = TRUCK_HEIGHT;
        this->velocity *= 1.4;
    }
    if(type == VehicleType::TRACTOR) {
        this->velocity *= 2.0;
    }
    if(type == VehicleType::POLICE) {
        this->height = TRUCK_HEIGHT;
        this->velocity = POLICE_SPEED;
    }
    this->wrecked = false;
}

void Car::draw(sf::RenderTarget* target, Assets& assets) const {
    if(type == VehicleType::ROCK) {
        assets.rock.draw(target, this->position);
    }
    else if(type == VehicleType::MOTORBIKE) {
        auto asset = assets.motorcycle[0];
        if(wrecked) {
            asset = assets.motorcycle[1];
        }
        asset.draw(target, this->position);
    }
    else if(type == VehicleType::TRUCK) {
        auto asset = assets.truck;
        asset.draw(target, this->position);
    }
    else if(type == VehicleType::TRACTOR) {
        auto asset = assets.tractor[0];
        if(wrecked) {
            asset = assets.tractor[1];
        }
        asset.draw(target, this->position);
    }
    else if(type == VehicleType::POLICE) {
        auto asset = assets.police;
        auto light = assets.police_lights[random()%2];
        if(wrecked) {
            // asset = assets.tractor[1];
        }
        asset.draw(target, this->position);
        light.draw(target, this->position);
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
        this->velocity -= (this->velocity - ROAD_SPEED) * WRECK_RETARDATION * delta * 2;
        this->position.y += delta * this->velocity;
    }
}
