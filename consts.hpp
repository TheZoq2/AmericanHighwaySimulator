#ifndef H_CONSTS
#define H_CONSTS

// Window parameters
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int WINDOW_CENTER = WINDOW_WIDTH / 2;


// Game parameters
const int CAR_AMOUNT = 10;
const int LANE_WIDTH = 200;
const int CAR_SPAWN_Y = -300;
const int CAR_DESPAWN_Y = WINDOW_HEIGHT - CAR_SPAWN_Y;
const int CAR_SPAWN_MAX_OFFSET = -CAR_SPAWN_Y + CAR_DESPAWN_Y;
const int CAR_SPEED = 100;
const int CAR_SPEED_VARIATION = 100;
const float ROAD_SPEED = 400;

const int PLAYER_WIDTH = 119;
const int PLAYER_HEIGHT = 249;
const float PLAYER_ACCELERATION_X = 1000;
const float PLAYER_ACCELERATION_Y = 300;
const float PLAYER_MAX_VEL_X = 1000;
const float PLAYER_OFFROAD_ACC_RETARDATION = 0.8;
const float PLAYER_OFFROAD_VEL_RETARDATION = 100;


#endif
