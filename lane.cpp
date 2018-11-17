#include "lane.hpp"
#include "consts.hpp"

Lane::Lane(sf::Vector2f position) {
    this->position = position;
}

void Lane::update(float delta) {
    this->position.y += delta;
}

void Lane::draw(sf::RenderTarget* target, Assets& assets) const {
    assets.lane.draw(target, this->position);
}
