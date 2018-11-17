#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <map>

namespace input {

    enum class Action { NONE, UP, DOWN, LEFT, RIGHT };

    class InputHandler {
    public:
        virtual float get_value(const Action ac) = 0;
    };


    class KeyboardInputHandler : public InputHandler {
    public:
        KeyboardInputHandler(
                sf::Keyboard::Key up,
                sf::Keyboard::Key down,
                sf::Keyboard::Key left,
                sf::Keyboard::Key right
                );

        float get_value(const Action ac);

    private:

        std::map<Action, sf::Keyboard::Key> action_map;
    };




    class ControllerInputHandler : public InputHandler {
    public:
        ControllerInputHandler(int id);

        float get_value(const Action ac);
    private:
        int id;
    };
}

#endif /* ifndef INPUT_H */
