#include "player.hpp"

#include <math.h>

Player::Player(std::string name, input::InputHandler& input_handler, 
        sf::Vector2f start_position) :
    name(name), position(start_position), input_handler(input_handler) {
        this->just_collided_with = nullptr;
    }

bool Player::is_pressed(const input::Action ac) {
    return this->input_handler.is_pressed(ac);
}

void Player::draw(sf::RenderTarget* target, Assets& assets) const {
    float visual_angle = atan2(0, velocity.x) * 10;

    assets.generic_car.draw(target, this->position, visual_angle);
}

