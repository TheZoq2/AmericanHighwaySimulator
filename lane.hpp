#ifndef LANE_H
#define LANE_H

#include <SFML/Graphics.hpp>

#include "assets.hpp"

class Lane {

public:
    Lane(sf::Vector2f position);

    void draw(sf::RenderTarget* target, Assets& assets) const;

    void update(float delta);

    sf::Vector2f position;
};

#endif /* ifndef LANE_H */
