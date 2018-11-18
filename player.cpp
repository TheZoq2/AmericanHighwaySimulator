#include "player.hpp"

#include <math.h>
#include <chrono>

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

bool Player::is_transparent() const {
    return this->transparency_time > 0;
}

void Player::draw(sf::RenderTarget* target, Assets& assets) const {
    float visual_angle = atan2(velocity.x, 100) * 10;

    auto shake = sf::Vector2f(0, 0);
    if(shake_left > 0) {
        shake = sf::Vector2f((random() % 600) / 100., (random() % 600) / 100.);
    }

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
    if (this->is_transparent()) {
        c = sf::Color(r, g, b, TRANSPARENCY_OPACITY);
    } else {
        c = sf::Color(r, g, b);
    }

    asset.draw(
        target,
        this->position + shake,
        visual_angle,
        c
    );

    if (this->powerup != nullptr) {
        sf::Vector2f powerup_pos = this->position + 
            sf::Vector2f{0, PLAYER_HEIGHT*0.7};
        this->powerup->draw_mini(target, assets, powerup_pos);
    }

    if (this->is_transparent()) {
        float amount = this->transparency_time/TRANSPARENCY_TIME;
        sf::RectangleShape transparency_bar
            {sf::Vector2f{amount*PLAYER_WIDTH, TRANSPARENCY_BAR_HEIGHT}};
        sf::Vector2f bar_pos = this->position + 
            sf::Vector2f{-PLAYER_WIDTH/2, PLAYER_HEIGHT*0.7};
        transparency_bar.setPosition(bar_pos);
        transparency_bar.setFillColor(sf::Color{50, 50, 255, 255});
        target->draw(transparency_bar);
    }

    this->draw_lights(target, visual_angle, assets);
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

void Player::draw_lights(
    sf::RenderTarget* target,
    float visual_angle,
    Assets& assets
) const {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds> (
            std::chrono::system_clock::now().time_since_epoch()
        );

    if(ms.count() % 500 > 250) {
        if (this->velocity.x > 0.2 * PLAYER_MAX_VEL_X) {
            assets.turn_right.draw(
                target,
                this->position,
                visual_angle
            );
        }
        if (this->velocity.x < -0.2 * PLAYER_MAX_VEL_X) {
            assets.turn_left.draw(
                target,
                this->position,
                visual_angle
            );
        }
    }
    if (this->input_handler->get_value(input::Action::DOWN) > 0.2) {
        assets.breaking.draw(
            target,
            this->position,
            visual_angle
        );
    }

    if(this->input_handler->get_value(input::Action::UP) > 0.3) {
        assets.outgas[ms.count() / 100 % 3].draw(
            target,
            this->position,
            visual_angle
        );
    }
}
