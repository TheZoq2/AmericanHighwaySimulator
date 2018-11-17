#ifndef PLAYER_H
#define PLAYER_H

#include "input.hpp"
#include "assets.hpp"
#include <string>
#include <SFML/Graphics.hpp>

class Player {

public:
    
    Player(std::string name, input::InputHandler& input_handler, 
            sf::Vector2f start_position);

    bool is_pressed(const input::Action ac);

    void draw(sf::RenderTarget* target, Assets& assets) const;

    std::string name;
    sf::Vector2f position;
    Player* just_collided_with;

private:

    input::InputHandler input_handler;

};

#endif /* ifndef PLAYER_H */
