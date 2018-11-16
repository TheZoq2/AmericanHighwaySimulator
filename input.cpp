#include "input.hpp"

input::InputHandler::InputHandler(std::map<sf::Keyboard, Action>& action_map) {
    this->action_map = action_map;
}

