#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>

#include "assets.hpp"

enum class VehicleType {
    CAR,
    TRUCK,
    MOTORBIKE,
    ROCK
};

class Car {

public:

    Car(VehicleType type, sf::Vector2f position);
    Car(sf::Vector2f position);

    void draw(sf::RenderTarget* target, Assets& assets) const;

    void update(float delta);

    sf::Vector2f position;

    int width;
    int height;
    bool wrecked;

    float velocity;

private:
    
    VehicleType type;

};

#endif /* ifndef CAR_H */
