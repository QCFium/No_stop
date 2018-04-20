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
#define ENABLE_LOG

// about game
#define MAX_SEC 60
#define POINTS_PER_CATCH 10

#define PLAYER_LIVES 3

#define MAX_BALLS 1000
#define BALL_INTERVAL_MSEC getBallIntervalMsec()

#define TREASURE_INTERVAL_MSEC 6000 // same as above, but for a treasure
#define WAIT_AFTER_SHOW_RESULT_MSEC 1000

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

int getBallIntervalMsec();
void complicate();
void uncomplicate();

int getScreenSizeX();
int getScreenSizeY();

bool initConfig();
