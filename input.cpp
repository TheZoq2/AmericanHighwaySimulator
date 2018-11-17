#include "input.hpp"

input::InputHandler::InputHandler(
                sf::Keyboard::Key up,
                sf::Keyboard::Key down,
                sf::Keyboard::Key left,
                sf::Keyboard::Key right) {
    this->action_map = {
        {Action::UP, up},
        {Action::DOWN, down},
        {Action::LEFT, left},
        {Action::RIGHT, right}
    };
}

bool input::InputHandler::is_pressed(const Action ac) {
    return sf::Keyboard::isKeyPressed(this->action_map[ac]);
}

