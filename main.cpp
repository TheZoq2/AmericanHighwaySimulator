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
    window.setPosition(sf::Vector2i(0, 0));

    srand(time(NULL));

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
                auto players = player_selection.players;
                auto player_amount = player_selection.players.size();
                for(std::size_t i = 0; i < player_amount; ++i) {
                    players[i].position.x = WINDOW_CENTER
                        + PLAYER_WIDTH * 2 * i
                        - PLAYER_WIDTH * player_amount;
                    
                    level.add_player(players[i]);
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
