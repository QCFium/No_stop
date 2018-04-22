#pragma once

#include <stdlib.h>
#include "config.h"

class Ball {
private:
	int rebounded_count = 0;

protected:
	int rebound_num = 0;
	int speed_x;
	int speed_y;

public:
	int x;
	int y;
	bool inScreen = true;
	bool isTreasure = false;

	int imageHandle = -1;
	int circle_r;

	// accessors
	Ball();
	void goNextPosition();
};


// balls
class TreasureBall : public Ball {
public:
	TreasureBall();
};

class OrangeBall : public Ball {
public:
	OrangeBall() {
		Ball();
		circle_r = 15;
	}
};

class PurpleBall : public Ball {
public:
	PurpleBall() {
		Ball();
		circle_r = 10;
	}
};

class YellowBall : public Ball {
public:
	YellowBall() {
		Ball();
		circle_r = 30;
	}
};

class BlueBall : public Ball {
public:
	BlueBall() {
		Ball();
		circle_r = 15;
		rebound_num = 1;
	}
};
