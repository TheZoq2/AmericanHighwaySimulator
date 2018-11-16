#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <map>
#include "car.hpp"
#include "player.hpp"
#include "input.hpp"

const float PLAYER_SPEED = 2.;

class Level {

public:

    Level(int num_lanes);
    ~Level();

    void draw(sf::RenderTarget* target, Assets& assets) const;
    void update();

    void add_player(Player& player);

private:

    void handle_input();

    int num_lanes;

    std::vector<Car> cars;
    std::vector<Player> players;

};

#endif /* ifndef LEVEL_H */

