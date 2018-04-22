#include <stdio.h>
#include <stdlib.h>

#include "ball.h"
#include "config.h"
#include "DxLib.h"

// balls
class TreasureBall : public Ball {
public:
	TreasureBall() {
		typeId = TYPE_TREASURE;
		circle_r = 20;
		rebound_num = 10000; // infinite
		isTreasure = true;
	}
};

class OrangeBall : public Ball {
public:
	OrangeBall() {
		typeId = TYPE_BALL0;
		circle_r = 15;
	}
};

class PurpleBall : public Ball {
public:
	PurpleBall() {
		typeId = TYPE_BALL1;
		circle_r = 10;
	}
};

class YellowBall : public Ball {
public:
	YellowBall() {
		typeId = TYPE_BALL2;
		circle_r = 30;
	}
};

class BlueBall : public Ball {
public:
	BlueBall() {
		typeId = TYPE_BALL3;
		circle_r = 15;
		rebound_num = 1;
	}
};


Ball* _getRandomBall() {
	int random = rand() % 4;
	if (random == 0) return (new OrangeBall());
	else if (random == 1) return (new PurpleBall());
	else if (random == 2) return (new YellowBall());
	else if (random == 3) return (new BlueBall());
	else return NULL; // unexpected
}

Ball* newBall() {
	static const int from_left = 0;
	static const int from_right = 1;
	static const int from_top = 2;
	static const int from_bottom = 3;

	static int ball_from = from_left;

	/* !! IMPORTANT !! */
	/* !! DELETE IT YOURSELF !! */
	Ball* ball = _getRandomBall();
	if (!ball) return NULL;

	// get next direction that the ball should come from
	if (ball_from == from_bottom) ball_from = from_left;
	else ball_from++;

	if (ball_from == from_left) {
		// last created ball's x was not 0 -> this time, it'll be 0
		ball->x = 0;
		ball->y = rand() % SCREEN_SIZE_Y;
		ball->speed_x = BALL_SPEED_RANDOM_FAST; // speed is always 2 - 6
		ball->speed_y = BALL_SPEED_RANDOM_SLOW; // move vertically slower
	} else if (ball_from == from_right) {
		// last created ball's x was not 0 -> this time, it'll be 0
		ball->x = SCREEN_SIZE_X;
		ball->y = rand() % SCREEN_SIZE_Y;
		ball->speed_x = -BALL_SPEED_RANDOM_FAST; // speed is always 2 - 6
		ball->speed_y = BALL_SPEED_RANDOM_SLOW; // move vertically
	} else if (ball_from == from_top) {
		ball->x = rand() % SCREEN_SIZE_X;
		ball->y = 0;
		ball->speed_x = BALL_SPEED_RANDOM_SLOW; // move horizontally slower
		ball->speed_y = BALL_SPEED_RANDOM_FAST; // speed is always 2 - 6
	}
	else if (ball_from == from_bottom) {
		ball->x = rand() % SCREEN_SIZE_X;
		ball->y = SCREEN_SIZE_Y;
		ball->speed_x = BALL_SPEED_RANDOM_SLOW; // move horizontally slower
		ball->speed_y = -BALL_SPEED_RANDOM_FAST; // speed is always 2 - 6
	}
	ball->rebounded_count = 0;
	return ball;
}

Ball* newTreasureBall() {
	/* !! IMPORTANT !! */
	/* !! DELETE IT YOURSELF !! */
	Ball* ball = new TreasureBall();
	if (!ball) return NULL;
	
	// treasures always start to move from the edge of windows
	ball->x = (rand() % 2) ? 0 : SCREEN_SIZE_X; // the leftest or the rightest
	ball->y = (rand() % 2) ? 0 : SCREEN_SIZE_Y; // the top or the bottom
	ball->speed_x = TREASURE_SPEED_RANDOM;
	ball->speed_y = TREASURE_SPEED_RANDOM;
	return ball;
}
