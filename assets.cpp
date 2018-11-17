#include "assets.hpp"

#include <iostream>

Asset::Asset(std::string path) {
    if(!this->texture.loadFromFile(path)) {
        std::cout << "Failed to load image " << path << std::endl;
        exit(-1);
    }

    this->sprite.setTexture(this->texture);
    this->sprite.setOrigin(texture.getSize().x / 2, texture.getSize().y / 2);
}




void Asset::draw(sf::RenderTarget* target, sf::Vector2f position) {
    draw(target, position, 0);
}
void Asset::draw(sf::RenderTarget* target, sf::Vector2f position, float angle) {
    draw(target, position, angle, sf::Color(255, 255, 255, 255));
}

void Asset::draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Color color) {
    sprite.setColor(color);
    sprite.setPosition(position);
    sprite.setRotation(angle);
    target->draw(sprite);
}

void Asset::set_origin(float x, float y) {
    auto size = texture.getSize();
    sprite.setOrigin(sf::Vector2f(size.x * x, size.y * y));
}


Assets::Assets() :
    generic_car("../resources/car.png"),
    lane("../resources/lane.png")
{
    lane.set_origin(0,0);
}
