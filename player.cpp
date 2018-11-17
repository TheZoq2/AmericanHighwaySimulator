#include "player.hpp"

Player::Player(std::string name, input::InputHandler& input_handler, 
        sf::Vector2f start_position) :
    name(name), position(start_position), input_handler(input_handler) { }

bool Player::is_pressed(const input::Action ac) {
    return this->input_handler.is_pressed(ac);
}


