#include <SFML/Window.hpp>

#include <chrono>
#include <iostream>

#include "level.hpp"
#include "assets.hpp"
#include "consts.hpp"
#include "input.hpp"
#include "player_selection.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "American highway simulator");

    Level level(5);
    Assets assets;

    PlayerSelection player_selection;

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

        window.clear(sf::Color::Black);

        if(!player_selection.done) {
            player_selection.draw(&window, assets);
            player_selection.run();

            if(player_selection.done) {
                for(auto player : player_selection.players) {
                    level.add_player(player);
                }
            }
        }
        else {
            level.update(next_time_step);

            level.draw(&window, assets);
        }




        window.display();

        auto frame_end = std::chrono::steady_clock::now().time_since_epoch();
        auto delta = std::chrono::duration_cast<FloatSeconds>(frame_end - frame_start);
        next_time_step = delta.count();
    }

    return 0;
}
