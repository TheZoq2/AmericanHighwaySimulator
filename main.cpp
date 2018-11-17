#include <SFML/Window.hpp>
#include "level.hpp"
#include "assets.hpp"
#include "consts.hpp"
#include "input.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "American highway simulator");

    Level level(5);
    Assets assets;
    input::InputHandler mh{
        sf::Keyboard::W,
        sf::Keyboard::S,
        sf::Keyboard::A,
        sf::Keyboard::D,
    };
    input::InputHandler ph{
        sf::Keyboard::Up,
        sf::Keyboard::Down,
        sf::Keyboard::Left,
        sf::Keyboard::Right,
    };
    Player mamma{"Din mamma", mh, sf::Vector2f{0, 0}};
    level.add_player(mamma);

    Player pappa{"Din pappa", ph, sf::Vector2f{0, 0}};
    level.add_player(pappa);

    while(window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        level.update();

// TODO implement timer based movement
        window.clear(sf::Color::Black);

        level.draw(&window, assets);

        window.display();
    }

    return 0;
}
