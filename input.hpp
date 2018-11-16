#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>
#include <map>

namespace input {

    enum class Action { NONE, UP, LEFT, RIGHT };

    class InputHandler {

    public:

        InputHandler(std::map<sf::Keyboard, Action>& action_map);

        bool is_pressed(Action ac) const;

    private:

        std::map<sf::Keyboard, Action> action_map;

    };

};

#endif /* ifndef INPUT_H */
