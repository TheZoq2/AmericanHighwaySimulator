#ifndef H_PLAYER_SELECTION
#define H_PLAYER_SELECTION

#include <SFML/Graphics.hpp>

#include <vector>
#include <set>

#include "input.hpp"
#include "player.hpp"

class PlayerSelection {
public:
    PlayerSelection();

    void draw(sf::RenderTarget* target, Assets& assets);

    void run(Assets& assets);

    std::vector<Player> players;

    bool done = false;

    int spawn_x = 200;

private:
    void add_player(input::InputHandler* handler, Assets& assets);

    input::KeyboardInputHandler* left;
    input::KeyboardInputHandler* right;
    sf::Texture background_texture;
    sf::Sprite background;

    std::set<int> used_controllers;
};



#endif
