#ifndef H_CONSTS
#define H_CONSTS

// Window parameters
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int WINDOW_CENTER = WINDOW_WIDTH / 2;


// Game parameters
const int CAR_AMOUNT = 14;
const int LANE_WIDTH = 200;
const int CAR_SPAWN_Y = -300;
const int CAR_DESPAWN_Y = WINDOW_HEIGHT - CAR_SPAWN_Y;
const int CAR_SPAWN_MAX_OFFSET = -CAR_SPAWN_Y + CAR_DESPAWN_Y;
const int CAR_SPEED = 100;
const int CAR_SPEED_VARIATION = 100;
const float ROAD_SPEED = 800;

const int PLAYER_WIDTH = 119;
const int PLAYER_HEIGHT = 249;
const float PLAYER_ACCELERATION_X = 1000;
const float PLAYER_ACCELERATION_Y = 700;
const float PLAYER_ACCELERATION_Y_DOWN = 1000;
const float PLAYER_MAX_VEL_X = 1000;
const float PLAYER_OFFROAD_ACC_RETARDATION = 0.6;
const float PLAYER_OFFROAD_VEL_RETARDATION = 70;

const int COLLISION_MAX_BREAKAGE = 500;
const float COLLISION_DAMAGE = 40;

const float POWERUP_SPAWN_PROBABILITY = 0.00;
const float POWERUP_ANGLE_SPEED = 100;
const int POWERUP_HEIGHT = 207;
const int POWERUP_WIDTH = 147;

#endif
