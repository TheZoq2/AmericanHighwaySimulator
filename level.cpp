#include "level.hpp"
#include <iostream>

Level::Level(int num_lanes) {
    this->num_lanes = num_lanes;
}

Level::~Level() { }

void Level::update() {

    handle_input();

    for (auto& player : players) {
        std::cout << player.name << ": " <<
            player.position.x << ", " << player.position.y << std::endl;
    }
}

void Level::handle_input() {
    for (auto& player : players) {
        int dx{0}, dy{0};
        if (player.is_pressed(input::Action::DOWN)) {
            dy -= PLAYER_SPEED;
        } else if (player.is_pressed(input::Action::UP)) {
            dy += PLAYER_SPEED;
        }
        if (player.is_pressed(input::Action::LEFT)) {
            dx += PLAYER_SPEED;
        } else if (player.is_pressed(input::Action::RIGHT)) {
            dx -= PLAYER_SPEED;
        }

        sf::Vector2f dxdy(dx, dy);
        player.position += dxdy;
    }
}

void Level::add_player(Player& player) {
    players.push_back(player);
}

