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

SoundAsset::SoundAsset(std::string path) {
    if(!this->buffer.loadFromFile(path)) {
        std::cout << "Failed to load sound " << path << std::endl;
        exit(-1);
    }
}

sf::Sound* SoundAsset::get_sound() {
    sf::Sound* sound = new sf::Sound;
    sound->setBuffer(this->buffer);
    return sound;
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
    },
    motorcycle{
        Asset("../resources/motorcycle.png"),
        Asset("../resources/motorcycle_crashed.png"),
    },
    truck("../resources/truck.png"),
    game_over("../resources/game_over.png"),
    tractor{
        Asset("../resources/tractor.png"),
        Asset("../resources/tractor_broken.png")
    },
    bmv("../resources/bmv.png"),
    crosshair("../resources/crosshair.png"),
    inverted("../resources/inverted.png"),
    police("../resources/police.png"),
    police_lights{
        Asset("../resources/police_lights_1.png"),
        Asset("../resources/police_lights_2.png"),
    },
    engine("../resources/audio/motor2.ogg"),
    music("../American highway simulator.ogg"),
    bmv_sound{
        SoundAsset("../resources/audio/bmw.ogg"),
        SoundAsset("../resources/audio/bmw_spak.ogg"),
    },
    bump{
        SoundAsset("../resources/audio/bump1.ogg"),
        SoundAsset("../resources/audio/bump2.ogg"),
    },
    crash{
        SoundAsset("../resources/audio/crash1.ogg"),
        SoundAsset("../resources/audio/crash2.ogg"),
        SoundAsset("../resources/audio/crash3.ogg"),
    },
    macke("../resources/audio/macke.ogg"),
    swear{
        SoundAsset("../resources/audio/norsk.ogg"),
        SoundAsset("../resources/audio/wanker.ogg"),
    },
    siren("../resources/audio/siren.ogg"),
    powerup_sound("../resources/audio/powerup.ogg")
{
    lane.set_origin(0,0);

    police_lights[0].set_origin(0.5,1);
    police_lights[1].set_origin(0.5,1);
}
