#include "config.h"
#include <Windows.h>

static int ball_interval_msec = 700;
static int display_x = 0;
static int display_y = 0;

bool initConfig() {
	display_x = GetSystemMetrics(SM_CXSCREEN);
	display_y = GetSystemMetrics(SM_CYSCREEN);
	return display_x && display_y;
}

int getScreenSizeX() { return display_x; }

int getScreenSizeY() { return display_y; }

int getBallIntervalMsec() { return ball_interval_msec; }
void complicate() { ball_interval_msec -= 50; }
void uncomplicate() { ball_interval_msec += 50; }
