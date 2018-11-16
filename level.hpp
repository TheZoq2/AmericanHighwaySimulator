#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "car.hpp"
#include "player.hpp"

class Level {

public:

    Level(int num_lanes);
    ~Level();

private:

    int num_lanes;

    std::vector<Car> cars;
    std::vector<Player> players;

};

#endif /* ifndef LEVEL_H */

