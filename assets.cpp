#include "assets.hpp"

#include <iostream>

Asset::Asset(std::string path) {
    if(!this->texture.loadFromFile(path)) {
        std::cout << "Failed to load image " << path << std::endl;
        exit(-1);
    }

    this->sprite.setTexture(this->texture);
}


void Asset::draw(sf::RenderTarget* target, sf::Vector2f position) {
    sprite.setPosition(position);
    target->draw(sprite);
}


Assets::Assets() :
    generic_car("../resources/car.png"),
    lane("../resources/lane.png")
{
}
