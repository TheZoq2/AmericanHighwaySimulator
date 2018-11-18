#ifndef PLAYER_H
#define PLAYER_H

#include "input.hpp"
#include "assets.hpp"
#include "consts.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include "powerup.hpp"
#include "car.hpp"

class Player {

public:
    
    Player(std::string name, input::InputHandler* input_handler, 
            sf::Vector2f start_position);

    void draw(sf::RenderTarget* target, Assets& assets) const;

    void set_powerup(PowerUp* p);
    void new_color();

    bool is_transparent() const;

    std::string name;
    sf::Vector2f position;
    Player* just_collided_with;
    sf::Vector2f velocity;

    float health = 100;
    sf::Vector2f persistent_acceleration;

    bool wrecked;

    PowerUp* powerup;

    input::InputHandler* input_handler;

    float invuln_time = 0;

    Car* collidee = nullptr;

    float transparency_time = 0;

    float shake_left = 0;

    bool selection_mode = false;
    bool target_selected = false;
    float selection_time = 0;

    Player* selected_by = nullptr;
    size_t selected_target_index;

    float r, g, b;
private:
    void draw_lights(sf::RenderTarget* target, float visual_angle, Assets& assets) const;


};

#endif /* ifndef PLAYER_H */
