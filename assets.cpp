#include "assets.hpp"

#include <iostream>

Asset::Asset(std::string path) {
    if(!this->texture.loadFromFile(path)) {
        std::cout << "Failed to load image " << path << std::endl;
        exit(-1);
    }

    this->sprite.setTexture(this->texture);
    this->sprite.setOrigin(PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2);
}


void Asset::draw(sf::RenderTarget* target, sf::Vector2f position) {
    draw(target, position, 0);
}
void Asset::draw(sf::RenderTarget* target, sf::Vector2f position, float angle) {
    sprite.setPosition(position);
    sprite.setRotation(angle);
    target->draw(sprite);
}


Assets::Assets() :
    generic_car("../resources/car.png"),
    lane("../resources/lane.png")
{
}
