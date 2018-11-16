#include <SFML/Window.hpp>
#include "level.hpp"

#include "level.hpp"
#include "assets.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1080, 1080), "American highway simulator");


    Level level(5);
    Assets assets;
    while(true) {
        level.update();

        window.clear(sf::Color::Black);

        level.draw(&window, assets);

        window.display();
    }

    return 0;
}
