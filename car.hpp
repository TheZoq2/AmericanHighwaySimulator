#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>

#include "assets.hpp"

enum class VehicleType {
    CAR,
    TRUCK,
    MOTORBIKE
};

class Car {

public:

    Car(VehicleType type, sf::Vector2f position);

    void draw(sf::RenderTarget* target, Assets& assets) const;

private:
    
    VehicleType type;

    sf::Vector2f position;

};

#endif /* ifndef CAR_H */
