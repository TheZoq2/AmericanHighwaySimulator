#include "powerup.hpp"
#include <cmath>

void PowerUp::draw(sf::RenderTarget* target, Assets& assets) const {
    switch (this->type) {
        case PowerUpType::SLEEPY:
            assets.sleep.draw(target, this->position, this->angle);
            break;
        case PowerUpType::TRANSPARENCY:
            assets.transparency.draw(target, this->position, this->angle);
            break;
    }
}

void PowerUp::draw_mini(sf::RenderTarget* target, Assets& assets,
        sf::Vector2f pos) const {
    float s = (std::sin(this->angle*POWERUP_BOUNCE_SPEED) + 3)/3;
    sf::Vector2f scale {s, s};
    switch (this->type) {
        case PowerUpType::SLEEPY:
            assets.sleep.draw(target, pos, this->angle, scale);
            break;
        case PowerUpType::TRANSPARENCY:
            assets.transparency.draw(target, pos, this->angle, scale);
            break;
    }
}

