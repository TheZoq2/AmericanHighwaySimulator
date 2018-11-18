#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "assets.hpp"

enum PowerUpType {
    SLEEPY, TRANSPARENCY, BMV, INVERTED,
    NUM_POWERUPS
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
