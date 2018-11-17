#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <map>
#include "car.hpp"
#include "player.hpp"
#include "input.hpp"
#include "consts.hpp"


struct CarCollisionResult {
    const bool collision_occurred;
    Player* p;
    Car* car;
};

class Level {

public:

    Level(int num_lanes);
    ~Level();

    void draw(sf::RenderTarget* target, Assets& assets) const;
    void update(float delta);

    void add_player(Player& player);

private:

    void handle_input(float delta_time);
    void spawn_car();

    /*
     * Checks if Player p can be moved to new_pos. If it can,
     * it returns a nullptr, otherwise it returns a pointer to the player
     * it collides with.
     */
    Player* get_colliding_player(const Player* p, sf::Vector2f new_pos);

    /*
     * Returns a CarCollisionResult containing information about
     * whether a player has collided with a car.
     */
    CarCollisionResult check_car_collisions();

    void on_player_collision_with_other(Player* collider, Player* collided);

    void on_player_collision_with_car(Player* p, Car* c);

    int num_lanes;

    std::vector<Car> cars;
    std::vector<Player> players;

    int lane_amount = 6;

    int road_width;
};

#endif /* ifndef LEVEL_H */

