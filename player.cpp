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
    this->angle = 0;
    this->powerup = nullptr;
    this->persistent_acceleration = sf::Vector2f(0, 0);

    new_color();
}


void Player::draw(sf::RenderTarget* target, Assets& assets) const {
    float angle = atan2(velocity.x, 100) * 10;

    assets.generic_car.draw(
        target,
        this->position,
        angle,
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


void Player::new_color() {
    r = random() % 255;
    g = random() % 255;
    b = random() % 255;
}
