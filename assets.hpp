#ifndef H_ASSETS
#define H_ASSETS

#include <SFML/Graphics.hpp>
#include <string>

class Asset {
public:
    Asset(std::string path);

    void draw(sf::RenderTarget* target, sf::Vector2f position);

    sf::Texture texture;
    sf::Sprite sprite;
};

class Assets {
public:
    Assets();

    Asset generic_car;
};
#endif
