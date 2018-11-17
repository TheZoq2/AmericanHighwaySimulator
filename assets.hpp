#ifndef H_ASSETS
#define H_ASSETS

#include <SFML/Graphics.hpp>
#include <string>

#include "consts.hpp"

class Asset {
public:
    Asset(std::string path);

    void draw(sf::RenderTarget* target, sf::Vector2f position);
    void draw(sf::RenderTarget* target, sf::Vector2f position, float angle);

    sf::Texture texture;
    sf::Sprite sprite;
};

class Assets {
public:
    Assets();

    Asset generic_car;
    Asset lane;
};
#endif
