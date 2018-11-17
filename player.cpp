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
    this->persistent_acceleration = sf::Vector2f(0, 0);

    new_color();
}


void Player::draw(sf::RenderTarget* target, Assets& assets) const {
    float visual_angle = atan2(velocity.x, 100) * 10;

    auto asset = assets.generic_car[0];
    if (wrecked){
        asset = assets.generic_car[3];
    }
    else if(health < 50) {
        asset = assets.generic_car[2];
    }
    else if(health < 75) {
        asset = assets.generic_car[1];
    }

    sf::Color c;
    if (this->transparency_time > 0) {
        c = sf::Color(r, g, b, TRANSPARENCY_OPACITY);
    } else {
        c = sf::Color(r, g, b);
    }

    asset.draw(
        target,
        this->position,
        visual_angle,
        c
    );

    if (this->powerup != nullptr) {
        sf::Vector2f powerup_pos = this->position + 
            sf::Vector2f{0, PLAYER_HEIGHT*0.7};
        this->powerup->draw_mini(target, assets, powerup_pos);
    }

    if (this->transparency_time > 0) {
        float amount = this->transparency_time/TRANSPARENCY_TIME;
        sf::RectangleShape transparency_bar
            {sf::Vector2f{amount*PLAYER_WIDTH, TRANSPARENCY_BAR_HEIGHT}};
        sf::Vector2f bar_pos = this->position + 
            sf::Vector2f{0, PLAYER_HEIGHT*0.7};
        transparency_bar.setPosition(bar_pos);
        transparency_bar.setFillColor(sf::Color{50, 50, 255, 255});
        target->draw(transparency_bar);
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
