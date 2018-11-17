#include "powerup.hpp"

void PowerUp::draw(sf::RenderTarget* target, Assets& assets) const {
    assets.powerup.draw(target, this->position, this->angle);
}

