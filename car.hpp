#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>

#include "assets.hpp"

enum class VehicleType {
    CAR,
    TRUCK,
    MOTORBIKE,
    TRACTOR,
    ROCK,
    POLICE
};

class Car {

public:

    Car(VehicleType type, sf::Vector2f position, Assets& assets);
    Car(sf::Vector2f position, Assets& assets);

    void draw(sf::RenderTarget* target, Assets& assets) const;

    void update(float delta, Assets& assets);

    sf::Vector2f position;

    sf::Sound* sound = nullptr;

    int width;
    int height;
    bool wrecked;

    float velocity;

    VehicleType type;
private:
    float police_delay = 2;
    bool police_ready = false;
};

#endif /* ifndef CAR_H */
