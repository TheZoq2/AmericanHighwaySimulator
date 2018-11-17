#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "assets.hpp"

const int NUM_POWERUPS = 2;
enum PowerUpType {
    SLEEPY, TRANSPARENCY
};

struct PowerUp {

    sf::Vector2f position;
    PowerUpType type;
    float angle;

    void draw(sf::RenderTarget* target, Assets& assets) const;

};

#endif /* ifndef POWERUP_H */
