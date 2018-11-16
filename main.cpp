#include <SFML/Window.hpp>
#include "level.hpp"
#include "assets.hpp"
#include "consts.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "American highway simulator");


    Level level(5);
    Assets assets;

    while(window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        level.update();

        window.clear(sf::Color::Black);

        level.draw(&window, assets);

        window.display();
    }

    return 0;
}
