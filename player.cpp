#include "player.hpp"

#include <math.h>

Player::Player(std::string name, input::InputHandler* input_handler, 
        sf::Vector2f start_position) :
    name(name), position(start_position) {
        this->just_collided_with = nullptr;
        this->wrecked = false;
        this->input_handler = input_handler;
        this->powerup = nullptr;
    }


void Player::draw(sf::RenderTarget* target, Assets& assets) const {
    float visual_angle = atan2(velocity.x, 100) * 10;

    assets.generic_car.draw(target, this->position, visual_angle);
}

void Player::set_powerup(PowerUp* p) {
    if (this->powerup != nullptr) {
        delete this->powerup;
    }
    this->powerup = p;
}

