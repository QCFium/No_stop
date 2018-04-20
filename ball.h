#pragma once

#include "config.h"

class Ball {
public:
	int x;
	int y;
	int speed_x;
	int speed_y;
	bool inScreen;
	bool isTreasure;
	int rebound_num;
	int rebounded_count;

	unsigned int color;
	int circle_r;

	// returns if the ball is still in the screen
	void goNextPosition() {
		x += speed_x;
		y += speed_y;

		// check if the ball is still in the window
		if (rebound_num > rebounded_count) {
			// fix position and rebound it
			if (x < 0) {
				x = 0;
				speed_x = -speed_x; // rebound
				rebounded_count++;
			} else if (x >= SCREEN_SIZE_X) {
				x = SCREEN_SIZE_X - 1;
				speed_x = -speed_x; // rebound
				rebounded_count++;
			}
			if (y < 0) {
				y = 0;
				speed_y = -speed_y; // rebound
				rebounded_count++;
			} else if (y >= SCREEN_SIZE_Y) {
				y = SCREEN_SIZE_Y - 1;
				speed_y = -speed_y; // rebound
				rebounded_count++;
			}
		} else {
			if (x < 0 || y < 0) inScreen = false;
			if (x >= SCREEN_SIZE_X + circle_r) inScreen = false; // completely out of screen
			if (y >= SCREEN_SIZE_Y + circle_r) inScreen = false; // completely out of screen
		}
	}

};

/* creates a new ball */
/* !! IMPORTANT !! */
/* DELETE the returned pointer YOURSELF */
Ball* newBall();

/* creates a new treasure ball */
/* !! IMPORTANT !! */
/* DELETE the returned pointer YOURSELF */
Ball* newTreasureBall();
