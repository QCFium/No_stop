#include "ball.h"

Ball::Ball() {
	// get next direction that the ball should come from
	int ball_from = rand() % 4;

	if (ball_from == 0) { // start moving from left
		this->x = 0;
		this->y = rand() % SCREEN_SIZE_Y;
		this->speed_x = BALL_SPEED_RANDOM_FAST; // move horizontally faster
		this->speed_y = BALL_SPEED_RANDOM_SLOW; // move vertically slower
	}
	else if (ball_from == 1) { // start moving from right
		this->x = SCREEN_SIZE_X;
		this->y = rand() % SCREEN_SIZE_Y;
		this->speed_x = -BALL_SPEED_RANDOM_FAST; // move horizontally faster
		this->speed_y = BALL_SPEED_RANDOM_SLOW; // move vertically
	}
	else if (ball_from == 2) { // start moving from top
		this->x = rand() % SCREEN_SIZE_X;
		this->y = 0;
		this->speed_x = BALL_SPEED_RANDOM_SLOW; // move horizontally slower
		this->speed_y = BALL_SPEED_RANDOM_FAST; // move vertically faster
	}
	else if (ball_from == 3) { // start moving from bottom
		this->x = rand() % SCREEN_SIZE_X;
		this->y = SCREEN_SIZE_Y;
		this->speed_x = BALL_SPEED_RANDOM_SLOW; // move horizontally slower
		this->speed_y = -BALL_SPEED_RANDOM_FAST; // move vertically faster
	}
}

void Ball::goNextPosition() {
	x += speed_x;
	y += speed_y;

	if (rebound_num > rebounded_count) { // check if still reboundable
		// fix position
		if (x < 0) x = 0;
		else if (x >= SCREEN_SIZE_X) x = SCREEN_SIZE_X - 1;
		if (y < 0) y = 0;
		else if (y >= SCREEN_SIZE_Y) y = SCREEN_SIZE_Y - 1;

		// rebound
		if (x == 0 || x == SCREEN_SIZE_X - 1) {
			speed_x = -speed_x;
			rebounded_count++;
		}
		if (y == 0 || y == SCREEN_SIZE_Y - 1) {
			speed_y = -speed_y;
			rebounded_count++;
		}
	} else {
		// check if it's still in screen
		if (x < -circle_r || y < -circle_r) inScreen = false;
		if (x >= SCREEN_SIZE_X + circle_r) inScreen = false;
		if (y >= SCREEN_SIZE_Y + circle_r) inScreen = false;
	}
}

TreasureBall::TreasureBall() {
	Ball();
	circle_r = 20;
	rebound_num = 10000; // infinite
	isTreasure = true;

	// treasures always start to move from the edge of windows
	this->x = (rand() % 2) ? 0 : SCREEN_SIZE_X; // the leftest or the rightest
	this->y = (rand() % 2) ? 0 : SCREEN_SIZE_Y; // the top or the bottom
	this->speed_x = TREASURE_SPEED_RANDOM;
	this->speed_y = TREASURE_SPEED_RANDOM;
}
