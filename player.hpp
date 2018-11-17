#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "input.hpp"

class Player {

public:
    
    Player(std::string name, input::InputHandler& input_handler, 
            sf::Vector2f start_position);

    bool is_pressed(const input::Action ac);

    std::string name;
    sf::Vector2f position;

private:

    input::InputHandler input_handler;

};

#endif /* ifndef PLAYER_H */
