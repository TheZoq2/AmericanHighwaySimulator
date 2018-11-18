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
const int ROCK_SIZE = 200;
const int MOTORCYCLE_WIDTH = 40;
const int MOTORCYCLE_HEIGHT = 100;
const int TRUCK_HEIGHT = 320;
const float POLICE_SPEED = -CAR_SPEED * 8;
const float POLICE_SPAWN_DISTANCE = 1500 + WINDOW_HEIGHT;

const int PLAYER_WIDTH = 119;
const int PLAYER_HEIGHT = 249;
const float PLAYER_ACCELERATION_X = 1000;
const float PLAYER_ACCELERATION_Y = 600;
const float PLAYER_ACCELERATION_Y_DOWN = 1000;
const float PLAYER_MAX_VEL_X = 500;
const float PLAYER_OFFROAD_ACC_RETARDATION = 0.6;
const float PLAYER_OFFROAD_VEL_RETARDATION = 70;
const float PLAYER_MAX_VEL_Y = 700;

const int COLLISION_MAX_BREAKAGE = 500;
const float COLLISION_DAMAGE = 40;

const float POWERUP_SPAWN_PROBABILITY = 0.01;
const float POWERUP_ANGLE_SPEED = 100;
const int POWERUP_HEIGHT = 70;
const int POWERUP_WIDTH = 70;
const float POWERUP_BOUNCE_SPEED = 0.05;
const float TRANSPARENCY_TIME = 3;
const float TRANSPARENCY_BAR_HEIGHT = 10;
const float TRANSPARENCY_OPACITY = 100;
const float BMV_TIME = 4;
const float BMV_ACC_MODIFIER = 2;

const float WRECK_RETARDATION = 2.;

const float TARGET_SELECTION_TIME = 1;
const float TARGET_SELECTION_ANGLE_SPEED = 100;

const float SLEEPY_TIME = 3;
const float INVERTED_TIME = 3;

#endif
