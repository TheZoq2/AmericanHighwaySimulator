#include "player_selection.hpp"

#include <iostream>

PlayerSelection::PlayerSelection() {
    this->background_texture.loadFromFile("../resources/intro.png");
    this->background.setTexture(background_texture);

    this->left = new input::KeyboardInputHandler {
        sf::Keyboard::W,
        sf::Keyboard::S,
        sf::Keyboard::A,
        sf::Keyboard::D,
    };
    this->right = new input::KeyboardInputHandler {
            sf::Keyboard::Up,
            sf::Keyboard::Down,
            sf::Keyboard::Left,
            sf::Keyboard::Right,
    };
}

void PlayerSelection::draw(sf::RenderTarget* target) {
    target->draw(this->background);
}

void PlayerSelection::run() {
    if (left != nullptr && left->get_value(input::Action::UP)) {
        std::cout << "player 1 joined" << std::endl;
        this->players.push_back(Player("Player", left, sf::Vector2f(spawn_x, 400)));
        left = nullptr;
        spawn_x += 300;
    }
    if (right != nullptr && right->get_value(input::Action::UP)) {
        std::cout << "player 2 joined" << std::endl;
        this->players.push_back(Player("Player", right, sf::Vector2f(spawn_x, 400)));
        right = nullptr;
        spawn_x += 300;
    }

    for(int i = 0; i < 8; ++i) {
        if(sf::Joystick::isConnected(i)) {
            if(sf::Joystick::isButtonPressed(i, 7)) {
                if(used_controllers.count(i) == 0) {
                    auto jh = new input::ControllerInputHandler(i);
                    this->players.push_back(Player(
                        "Player",
                        jh,
                        sf::Vector2f(spawn_x, 400)
                    ));
                    used_controllers.insert(i);
                }
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
        done = true;
    }
}
