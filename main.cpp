#include <SFML/Window.hpp>
#include "level.hpp"

int main() {
    sf::Window window(sf::VideoMode(1080, 1080), "American highway simulator");

    Level level{3};

    while(true) {
        level.update();
    }

    return 0;
}
