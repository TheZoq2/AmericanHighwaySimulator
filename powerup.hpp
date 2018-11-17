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
    void draw_mini(sf::RenderTarget* target, Assets& assets,
            sf::Vector2f pos) const;

};

#endif /* ifndef POWERUP_H */
