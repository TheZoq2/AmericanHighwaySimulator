#include <SFML/Window.hpp>

#include <chrono>
#include <iostream>

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
    Player mamma{"Din mamma", mh, sf::Vector2f{500, 400}};
    level.add_player(mamma);

    Player pappa{"Din pappa", ph, sf::Vector2f{1000, 400}};
    level.add_player(pappa);

    typedef std::chrono::duration<float> FloatSeconds;
    float next_time_step = 0;
    while(window.isOpen()) {
        auto frame_start = std::chrono::steady_clock::now().time_since_epoch();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        level.update(next_time_step);

        window.clear(sf::Color::Black);

        level.draw(&window, assets);

        window.display();

        auto frame_end = std::chrono::steady_clock::now().time_since_epoch();
        auto delta = std::chrono::duration_cast<FloatSeconds>(frame_end - frame_start);
        next_time_step = delta.count();

        std::cout << next_time_step << std::endl;
    }

    return 0;
}
