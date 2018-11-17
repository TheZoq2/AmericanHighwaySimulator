#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <map>
#include "car.hpp"
#include "player.hpp"
#include "input.hpp"
#include "lane.hpp"

const float PLAYER_SPEED = 2.;

class Level {

public:

    Level(int num_lanes);
    ~Level();

    void draw(sf::RenderTarget* target, Assets& assets) const;
    void update(float delta);

    void add_player(Player& player);

private:

    void handle_input();
    void spawn_car();
    void add_lane(int lane_num);

    /*
     * Checks if Player p can be moved to new_pos. If it can,
     * it returns a nullptr, otherwise it returns a pointer to the player
     * it collides with.
     */
    Player* get_colliding_player(const Player* p, sf::Vector2f new_pos);

    void on_player_collision_with_other(Player* collider, Player* collided);

    int num_lanes;

    std::vector<Car> cars;
    std::vector<Player> players;
    std::vector<Lane> lanes;

    int lane_amount = 7;

    int road_width;
};

#endif /* ifndef LEVEL_H */

