#pragma once

// application name
#define APPLICATION_NAME "止まれませーん"

// about screen
// #define WINDOW_MODE
#ifdef WINDOW_MODE
#define SCREEN_SIZE_X 1366
#define SCREEN_SIZE_Y 768
#else // WINDOW_MODE
#define SCREEN_SIZE_X getScreenSizeX()
#define SCREEN_SIZE_Y getScreenSizeY()
#endif // WINDOW_MODE

// logging
// #define ENABLE_LOG

// about game
#define POINTS_PER_CATCH 10

/* distribution mode */
// #define DIST_MODE
#ifdef DIST_MODE
#	define PLAYER_LIVES 5

#	define MAX_BALLS 2000

#	define MAX_SEC 180

// ball creation interval
#	define BALL_LVL1_START_SEC 20
#	define BALL_LVL2_START_SEC 140
#	define BALL_LVL0_INTERVAL_DEFAULT_MSEC 1000
#	define BALL_LVL1_INTERVAL_DEFAULT_MSEC 700
#	define BALL_LVL2_INTERVAL_DEFAULT_MSEC 400

// treasure creation interval
#	define TREASURE_LVL1_START_SEC 20
#	define TREASURE_LVL2_START_SEC 140
#	define TREASURE_LVL0_INTERVAL_DEFAULT_MSEC 6000
#	define TREASURE_LVL1_INTERVAL_DEFAULT_MSEC 7000
#	define TREASURE_LVL2_INTERVAL_DEFAULT_MSEC 4000

#else // DIST_MODE

#	define PLAYER_LIVES 3

#	define MAX_BALLS 1000

#	define MAX_SEC 60

// ball creation interval
#	define BALL_LVL1_START_SEC 15
#	define BALL_LVL2_START_SEC 40
#	define BALL_LVL0_INTERVAL_DEFAULT_MSEC 1200
#	define BALL_LVL1_INTERVAL_DEFAULT_MSEC 900
#	define BALL_LVL2_INTERVAL_DEFAULT_MSEC 300

// treasure creation interval
#	define TREASURE_LVL1_START_SEC 15
#	define TREASURE_LVL2_START_SEC 40
#	define TREASURE_LVL0_INTERVAL_DEFAULT_MSEC 3000
#	define TREASURE_LVL1_INTERVAL_DEFAULT_MSEC 5000
#	define TREASURE_LVL2_INTERVAL_DEFAULT_MSEC 2000

#endif // DIST_MODE

#define WAIT_AFTER_SHOW_RESULT_MSEC 1000

// resource dir
#define RESOURCE_DIR_NAME "res"

// the speed of balls
#define BALL_SPEED_RANDOM_FAST (rand() % 6 + 1)
#define BALL_SPEED_RANDOM_SLOW (rand() % 5 - 2)

// about the player
#define PLAYER_CIRCLE_R 15
#define PLAYER_SPEED_MAX 10
#define PLAYER_SPEED_CHANGE 1

// about blinking animation
#define DEAD_PLAYER_BLINK_INTERVAL_MSEC 300
#define DEAD_PLAYER_BLINK_NUM 5
#define SCORE_BLINK_INTERVAL_MSEC 300
#define SCORE_BLINK_NUM 5

// colors
#define COLOR_PLAYER     GetColor(255, 255, 255)
#define COLOR_PLAYER_HIT GetColor(255, 0, 0)
#define COLOR_MSG        GetColor(255, 255, 255)
#define COLOR_LOW_TIME   GetColor(255, 0, 0)
#define COLOR_SCORE_INCREASING GetColor(0, 255, 0)

// result string
#define GAMEOVER_STR "ゲームオーバー"
#define TIMEOVER_STR "クリア(%d秒間生き残りました)"
#define SCORE_STR "スコア : "
#define ANYKEY_STR "何かキーを押してください..."

int getBallIntervalMsec(int start_time);
int getTreasureIntervalMsec(int start_time);
int getBallIntervalAdjustion();
void complicate();
void uncomplicate();

int getScreenSizeX();
int getScreenSizeY();

bool initConfig();
