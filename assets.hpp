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
    void draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Vector2f scale);
    void draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Color color);
    void draw(sf::RenderTarget* target, sf::Vector2f position, float angle, sf::Color color, sf::Vector2f scale);

    void set_origin(float x, float y);

    sf::Texture texture;
    sf::Sprite sprite;
};

class Assets {
public:
    Assets();

    Asset generic_car[4];
    Asset lane;
    Asset powerup;
    Asset sleep;
    Asset transparency;
    Asset rock;
    Asset turn_left;
    Asset turn_right;
    Asset breaking;
    Asset outgas[3];
    Asset motorcycle[2];
    Asset truck;
    Asset tractor[2];
    Asset bmv;
    Asset crosshair;
};
#endif
