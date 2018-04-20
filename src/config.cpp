#include "DxLib.h"
#include <Windows.h>

#include "config.h"

static int ball_interval_adjust = 0;
static int display_x = 0;
static int display_y = 0;

bool initConfig() {
	display_x = GetSystemMetrics(SM_CXSCREEN);
	display_y = GetSystemMetrics(SM_CYSCREEN);
	return display_x && display_y;
}

int getScreenSizeX() { return display_x; }
int getScreenSizeY() { return display_y; }
int getBallIntervalAdjustion() { return ball_interval_adjust; }

int getBallIntervalMsec(int start_time) {
	int time_elapsed_sec = (GetNowCount() - start_time) / 1000;
	if (time_elapsed_sec > BALL_LVL2_START_SEC) return BALL_LVL2_INTERVAL_DEFAULT_MSEC + ball_interval_adjust;
	else if (time_elapsed_sec > BALL_LVL1_START_SEC) return BALL_LVL1_INTERVAL_DEFAULT_MSEC + ball_interval_adjust;
	else return BALL_LVL0_INTERVAL_DEFAULT_MSEC + ball_interval_adjust;
}
int getTreasureIntervalMsec(int start_time) {
	int time_elapsed_sec = (GetNowCount() - start_time) / 1000;
	if (time_elapsed_sec > TREASURE_LVL2_START_SEC) return TREASURE_LVL2_INTERVAL_DEFAULT_MSEC;
	else if (time_elapsed_sec > TREASURE_LVL1_START_SEC) return TREASURE_LVL1_INTERVAL_DEFAULT_MSEC;
	else return TREASURE_LVL0_INTERVAL_DEFAULT_MSEC;
}

void complicate() { ball_interval_adjust -= 50; }
void uncomplicate() { ball_interval_adjust += 50; }
