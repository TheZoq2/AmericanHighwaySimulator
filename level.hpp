#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <map>
#include "car.hpp"
#include "player.hpp"
#include "input.hpp"
#include "lane.hpp"
#include "consts.hpp"
#include "powerup.hpp"


struct CarCollisionResult {
    const bool collision_occurred;
    Player* p;
    Car* car;
};

class Level {

public:

    Level(int num_lanes, Assets& assets);
    ~Level();

    void draw(sf::RenderTarget* target, Assets& assets) const;
    void update(float delta, Assets& assets);

    void add_player(Player& player);

    int get_players_left();
    void reset_players();
    void reset_cars();

    float game_over_timeout = 3;

private:

    void update_players_handle_input(float delta_time, Assets& assets);
    void spawn_car(Assets& assets);
    void add_lane(int lane_num);

    void spawn_powerup();

    bool is_offroad(sf::Vector2f pos, int width) const;

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

    void on_player_collision_with_other(Player* collider, Player* collided,
            Assets& assets);

    void on_player_collision_with_car(Player* p, Car* c, Assets& assets);

    void check_if_players_within_bounds();

    void update_player_status(sf::RenderTarget* target, Assets& assets);

    int num_lanes;

    sf::Sound* macke;
    sf::Sound* police;
    sf::Sound* bump;
    sf::Sound* crash;
    sf::Sound* powerup_sound;
    sf::Sound* bmw1;
    sf::Sound* bmw2;

    std::vector<Car> cars;
    std::vector<PowerUp> powerups;
    std::vector<Player> players;
    std::vector<Lane> lanes;

    int lane_amount = 6;

    int road_width;

    bool someone_selecting = false;

    float selection_timeout;


    /////////////////////////////////////////
    // Powerups
    /////////////////////////////////////////
    
    void fire_player_powerup(Player* p);
    bool powerup_collides_with_player(PowerUp* pu, Player* p) const;
    void update_and_spawn_powerups(float delta_time);
    void draw_powerups(sf::RenderTarget* target, Assets& assets);
    
    void activate_target_selection(Player* p);
    void deactivate_target_selection(Player* p);
    void update_target_selection(Player* p, float delta_time);
    void update_sleepiness(Player* p, float delta_time);
    void update_invertedness(Player* p, float delta_time);
    void activate_sleepy_powerup(Player* p);
    void actually_fire_sleepy_powerup(Player* p);
    void actually_fire_inverted_powerup(Player* p);
    void activate_transparency_powerup(Player* p);
    void activate_bmv_powerup(Player* p);
    void activate_inverted_powerup(Player* p);
    void car_on_car_collision();
};

#endif /* ifndef LEVEL_H */

