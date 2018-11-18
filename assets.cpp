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

void Asset::draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Vector2f scale) {
    draw(target, position, angle, sf::Color(255, 255, 255, 255), scale);
}

void Asset::draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Color color) {
    draw(target, position, angle, color, sf::Vector2f{1, 1});
}

void Asset::draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Color color, sf::Vector2f scale) {
    sprite.setColor(color);
    sprite.setPosition(position);
    sprite.setRotation(angle);
    sprite.setScale(scale.x, scale.y);
    target->draw(sprite);
}

void Asset::set_origin(float x, float y) {
    auto size = texture.getSize();
    sprite.setOrigin(sf::Vector2f(size.x * x, size.y * y));
}


Assets::Assets() :
    generic_car{
        Asset("../resources/car.png"),
        Asset("../resources/car_hit.png"),
        Asset("../resources/car_damaged.png"),
        Asset("../resources/car_broken_down.png")
    },
    lane("../resources/lane.png"),
    powerup("../resources/powerup.png"),
    sleep("../resources/sleep.png"),
    transparency("../resources/transparency.png"),
    rock("../resources/rock.png"),
    turn_left("../resources/turn_left.png"),
    turn_right("../resources/turn_right.png"),
    breaking("../resources/breaking.png"),
    outgas {
        Asset("../resources/outgas1.png"),
        Asset("../resources/outgas2.png"),
        Asset("../resources/outgas3.png")
    }
{
    lane.set_origin(0,0);
}
