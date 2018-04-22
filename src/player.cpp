#include <stdlib.h>
#include "DxLib.h"

#include "player.h"
#include "ball.h"
#include "config.h"

static int x;
static int y;
static int speed_x;
static int speed_y;
static int score;
static int life_left;
static int last_damaged_time;

static int last_increased_time;

int getPlayerX() { return x; }
int getPlayerY() { return y; }
int getPlayerScore() { return score; }
int getLifeNumLeft() { return life_left; }

void increasePlayerScore() { 
	score += POINTS_PER_CATCH;
	last_increased_time = GetNowCount();
}

void initPlayer() {
	x = SCREEN_SIZE_X / 2; // position it to the center
	y = SCREEN_SIZE_Y / 2; // position it to the center
	speed_x = 0;
	speed_y = 0;
	score = 0;
	life_left = PLAYER_LIVES;
	last_damaged_time = 0;
	last_increased_time = 0;
}

bool killPlayer() {
	last_damaged_time = GetNowCount();
	if (--life_left <= 0) return true; // game over
	return false;
}


int getTimeElapsedFromLastDamage() {
	return last_damaged_time ? (GetNowCount() - last_damaged_time) : -1;
}

int getTimeElapsedFromLastIncrease() {
	return last_increased_time ? (GetNowCount() - last_increased_time) : -1;
}

void playerGoNextPosition(char* key_state) {
	// change speed depending on key inputs
	if (KEY_RIGHT(key_state)) speed_x += PLAYER_SPEED_CHANGE;
	else if (KEY_LEFT(key_state)) speed_x -= PLAYER_SPEED_CHANGE;
	if (KEY_DOWN(key_state)) speed_y += PLAYER_SPEED_CHANGE;
	else if (KEY_UP(key_state)) speed_y -= PLAYER_SPEED_CHANGE;

	// DON'T STOP - the speed will never stay at 0
	if (!KEY_LEFT(key_state) && !KEY_RIGHT(key_state) && !speed_x) speed_x = (rand() % 2) ? 1 : -1; // randomly 1 or -1
	if (!KEY_UP(key_state) && !KEY_DOWN(key_state) && !speed_y) speed_y = (rand() % 2) ? 1 : -1; // randomly 1 or -1

	// fix speed
	if (speed_x > PLAYER_SPEED_MAX) speed_x = PLAYER_SPEED_MAX;
	if (speed_y > PLAYER_SPEED_MAX) speed_y = PLAYER_SPEED_MAX;
	if (speed_x < -PLAYER_SPEED_MAX) speed_x = -PLAYER_SPEED_MAX;
	if (speed_y < -PLAYER_SPEED_MAX) speed_y = -PLAYER_SPEED_MAX;

	// go to the next position
	x += speed_x;
	y += speed_y;

	// fix position
	if (x >= SCREEN_SIZE_X) x = SCREEN_SIZE_X - 1;
	else if (x < 0) x = 0;
	if (y >= SCREEN_SIZE_Y) y = SCREEN_SIZE_Y - 1;
	else if (y < 0) y = 0;

	// fix speed again(if reached to the edge, rebound a little)
	if (x == 0 || x >= SCREEN_SIZE_X - 1) speed_x /= -2;
	if (y == 0 || y >= SCREEN_SIZE_Y - 1) speed_y /= -2;
}
