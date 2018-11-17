#include "player.hpp"

#include <math.h>

Player::Player(
    std::string name,
    input::InputHandler* input_handler,
    sf::Vector2f start_position
) : name(name), position(start_position)
{
    this->just_collided_with = nullptr;
    this->input_handler = input_handler;
    this->wrecked = false;
    this->powerup = nullptr;

    r = random() % 255;
    g = random() % 255;
    b = random() % 255;
}


void Player::draw(sf::RenderTarget* target, Assets& assets) const {
    float visual_angle = atan2(velocity.x, 100) * 10;

    assets.generic_car.draw(
        target,
        this->position,
        visual_angle,
        sf::Color(r, g, b)
    );

    if (this->powerup != nullptr) {
        sf::Vector2f powerup_pos = this->position + 
            sf::Vector2f{0, PLAYER_HEIGHT*0.7};
        this->powerup->draw_mini(target, assets, powerup_pos);
    }
}

void Player::set_powerup(PowerUp* p) {
    if (this->powerup != nullptr) {
        delete this->powerup;
    }
    this->powerup = p;
}

