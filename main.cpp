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
    sf::Texture bg_texture;
    bg_texture.setRepeated(true);
    bg_texture.loadFromFile("../resources/grass.png");

    sf::Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setTextureRect(sf::IntRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT));

    sf::Sprite bg_sprite_second;
    bg_sprite_second.setTexture(bg_texture);
    bg_sprite_second.setTextureRect(sf::IntRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT));
    bg_sprite_second.setPosition(sf::Vector2f(0, -WINDOW_HEIGHT));

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
            sf::Vector2f bg_pos = bg_sprite.getPosition();
            sf::Vector2f bg_second_pos = bg_sprite_second.getPosition();

            if (bg_pos.y > WINDOW_HEIGHT) {
                bg_sprite.setPosition(bg_pos.x, -WINDOW_HEIGHT);
            }
            else {
                bg_sprite.setPosition(bg_pos.x, bg_pos.y + 1);
            }

            if (bg_second_pos.y > WINDOW_HEIGHT) {
                bg_sprite_second.setPosition(bg_second_pos.x, -WINDOW_HEIGHT);
            }
            else {
                bg_sprite_second.setPosition(bg_second_pos.x, bg_second_pos.y + 1);
            }
            window.draw(bg_sprite);
            window.draw(bg_sprite_second);

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
