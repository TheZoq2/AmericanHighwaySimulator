#include "input.hpp"

using namespace input;

KeyboardInputHandler::KeyboardInputHandler(
                sf::Keyboard::Key up,
                sf::Keyboard::Key down,
                sf::Keyboard::Key left,
                sf::Keyboard::Key right,
                sf::Keyboard::Key fire) {
    this->action_map = {
        {Action::UP, up},
        {Action::DOWN, down},
        {Action::LEFT, left},
        {Action::RIGHT, right},
        {Action::FIRE, fire}
    };
}

float KeyboardInputHandler::get_value(const Action ac) {
    return sf::Keyboard::isKeyPressed(this->action_map[ac]);
}






ControllerInputHandler::ControllerInputHandler(int id) :
    id{id}
{}

float ControllerInputHandler::get_value(const Action ac) {
    auto axis_x = sf::Joystick::getAxisPosition(id, sf::Joystick::X) / 100;
    auto axis_y = sf::Joystick::getAxisPosition(id, sf::Joystick::Y) / 100;

    switch (ac) {
        case Action::UP:
            if (axis_y < 0) {
                return -axis_y;
            }
            break;
        case Action::DOWN:
            if (axis_y > 0) {
                return axis_y;
            }
            break;
        case Action::LEFT:
            if (axis_x < 0) {
                return -axis_x;
            }
            break;
        case Action::RIGHT:
            if (axis_x > 0) {
                return axis_x;
            }
            break;
        case Action::NONE:
            return 0;
            break;
        case Action::FIRE:
            if (sf::Joystick::isButtonPressed(this->id, 2)) {
                return 1;
            }
            break;
    }
    return 0;
}
