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
    window.setVerticalSyncEnabled(true);
    sf::Texture bg_texture;
    bg_texture.setRepeated(true);
    bg_texture.loadFromFile("../resources/grass.png");

    sf::Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setTextureRect(sf::IntRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT+100));
    bg_sprite.setOrigin(sf::Vector2f(0,0));

    sf::Sprite bg_sprite_second;
    bg_sprite_second.setTexture(bg_texture);
    bg_sprite_second.setTextureRect(sf::IntRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT+100));
    bg_sprite_second.setPosition(sf::Vector2f(0, -WINDOW_HEIGHT-10));
    bg_sprite_second.setOrigin(sf::Vector2f(0,0));

    window.setPosition(sf::Vector2i(0, 0));

    srand(time(NULL));

    Assets assets;
    Level level(5, assets);
    sf::Sound* music = assets.music.get_sound();
    music->setLoop(true);
    music->setVolume(80.f);
    music->play();

    PlayerSelection player_selection;

    typedef std::chrono::duration<float> FloatSeconds;
    float next_time_step = 0;

    bool is_game_over = false;

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
            player_selection.run(assets);
            level.reset_players();
            level.reset_cars();
            level.game_over_timeout = 3;
            is_game_over = false;

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
        else if (level.game_over_timeout <= 0 && !is_game_over) {
            assets.game_over.set_origin(0,0);
            assets.game_over.draw(&window, sf::Vector2f(0,0));

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && player_selection.done) {
                player_selection.done = false;
            }
        }
        else {
            sf::Vector2f bg_pos = bg_sprite.getPosition();
            sf::Vector2f bg_second_pos = bg_sprite_second.getPosition();

            if (bg_pos.y > WINDOW_HEIGHT) {
                bg_sprite.setPosition(bg_pos.x, -WINDOW_HEIGHT);
            }
            else {
                bg_sprite.setPosition(bg_pos.x, bg_pos.y + ROAD_SPEED * next_time_step);
            }

            if (bg_second_pos.y > WINDOW_HEIGHT) {
                bg_sprite_second.setPosition(bg_second_pos.x, -WINDOW_HEIGHT);
            }
            else {
                bg_sprite_second.setPosition(
                    bg_second_pos.x,
                    bg_second_pos.y + ROAD_SPEED * next_time_step
                );
            }
            window.draw(bg_sprite);
            window.draw(bg_sprite_second);

            level.update(next_time_step, assets);

            level.draw(&window, assets);
        }

        window.display();

        auto frame_end = std::chrono::steady_clock::now().time_since_epoch();
        auto delta = std::chrono::duration_cast<FloatSeconds>(frame_end - frame_start);
        next_time_step = delta.count();
    }

    return 0;
}
