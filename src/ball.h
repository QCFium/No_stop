#pragma once

#include "config.h"

typedef enum {
	TYPE_TREASURE,
	TYPE_BALL0,
	TYPE_BALL1,
	TYPE_BALL2,
	TYPE_BALL3
} BallType;

class Ball {
public:
	BallType typeId;
	int x;
	int y;
	int speed_x;
	int speed_y;
	bool inScreen = true;
	bool isTreasure = false;
	int rebound_num = 0;
	int rebounded_count;

	int imageHandle = -1;
	int circle_r;

	// returns if the ball is still in the screen
	void goNextPosition() {
		x += speed_x;
		y += speed_y;

		// check if the ball is still in the window
		if (rebound_num > rebounded_count) {
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
			if (x < -circle_r || y < -circle_r) inScreen = false;
			if (x >= SCREEN_SIZE_X + circle_r) inScreen = false; // completely out of screen
			if (y >= SCREEN_SIZE_Y + circle_r) inScreen = false; // completely out of screen
		}
	}

};

/*
	creates a new ball
	!! IMPORTANT !!
	DELETE the returned pointer YOURSELF
*/
Ball* newBall();

/* 
	creates a new treasure ball
	!! IMPORTANT !!
	DELETE the returned pointer YOURSELF
*/
Ball* newTreasureBall();
