#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <map>
#include "car.hpp"
#include "player.hpp"
#include "input.hpp"

class Level {

public:

    Level(int num_lanes);
    ~Level();

    void update();

private:

    int num_lanes;

    std::vector<Car> cars;
    std::vector<Player> players;

};

#endif /* ifndef LEVEL_H */

