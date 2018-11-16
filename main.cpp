#include <SFML/Window.hpp>
#include "level.hpp"
#include <unistd.h>

int main() {
    sf::Window window(sf::VideoMode(1080, 1080), "American highway simulator");

    Level level{3};

    while(true) {
        level.update();
        usleep(100000);
    }

    return 0;
}
