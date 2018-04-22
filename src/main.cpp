#include <stdlib.h>
#include "DxLib.h"

#include "config.h"
#include "ball.h"
#include "player.h"
#include "draw.h"
#include "error.h"

// graphic handles
int heart_handle;
int treasure_handle;
int ball0_handle;
int ball1_handle;
int ball2_handle;
int ball3_handle;

#define TREASURE_INTERVAL_MSEC(t) getTreasureIntervalMsec(t)
#define BALL_INTERVAL_MSEC(t) getBallIntervalMsec(t)

bool _initWindow() {
#ifndef ENABLE_LOG
	SetOutApplicationLogValidFlag(FALSE); // disable logging into a file
#endif

	if (SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32) != DX_CHANGESCREEN_OK) return false;

#ifdef WINDOW_MODE
	SetDoubleStartValidFlag(TRUE); // allow double starting
	if (
		ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK ||
		SetWindowSize(SCREEN_SIZE_X, SCREEN_SIZE_Y) == -1 ||
		SetWindowSizeChangeEnableFlag(FALSE, FALSE) == -1) ||
		SetWindowText(APPLICATION_NAME) != 0)
		return false;
#endif

	if (DxLib_Init() == -1 || SetDrawScreen(DX_SCREEN_BACK) == -1) // draw to backside
		return false;

	return true;
}

bool _loadImages() {
	heart_handle    = LoadGraph(RESOURCE_DIR_NAME "/heart.png");
	treasure_handle = LoadGraph(RESOURCE_DIR_NAME "/treasure.png");
	ball0_handle    = LoadGraph(RESOURCE_DIR_NAME "/ball0.png");
	ball1_handle    = LoadGraph(RESOURCE_DIR_NAME "/ball1.png");
	ball2_handle    = LoadGraph(RESOURCE_DIR_NAME "/ball2.png");
	ball3_handle    = LoadGraph(RESOURCE_DIR_NAME "/ball3.png");
	return (heart_handle != -1 && treasure_handle != -1 &&
			ball0_handle != -1 && ball1_handle != -1 &&
			ball2_handle != -1 && ball3_handle != -1);
}

void _freeResources() {
	// unload graphics
	DeleteGraph(heart_handle);
	DeleteGraph(treasure_handle);
	DeleteGraph(ball0_handle);
	DeleteGraph(ball1_handle);
	DeleteGraph(ball2_handle);
	DeleteGraph(ball3_handle);

	// just to be safe
	heart_handle = -1;
	treasure_handle = -1;
	ball0_handle = -1;
	ball1_handle = -1;
	ball2_handle = -1;
	ball3_handle = -1;
}


void _deleteBalls(Ball* balls[]) {
	for (int i = 0; i < MAX_BALLS; i++) {
		delete balls[i]; // delete NULL; does nothing
		balls[i] = NULL; // just to be safe
	}
}

/* return if it's game over */
bool _judgeHits(Ball* balls[]) {
	for (int i = 0; i < MAX_BALLS; i++) {
		if (!balls[i]) return false; // no more ball
		if (!balls[i]->inScreen) continue; // the ball has already gone out from the screen

		// get distance of x and that of y
		int distance_x = balls[i]->x - getPlayerX();
		if (distance_x < 0) distance_x = -distance_x;
		int distance_y = balls[i]->y - getPlayerY();
		if (distance_y < 0) distance_y = -distance_y;

		// use Archimedes' theorem(!?) to judge
		int r_sum = PLAYER_CIRCLE_R + balls[i]->circle_r;
		bool hit = (r_sum * r_sum > (distance_x * distance_x) + (distance_y * distance_y));

		if (balls[i]->isTreasure) {
			if (hit) {
				balls[i]->inScreen = false; // it's gotten
				increasePlayerScore();
				return false; // it's not game over
			}
		} else {
			if (hit) return ((balls[i]->inScreen = killPlayer())); // if player still alive, remove the ball from screen
		}
	}
	return false;
}

Ball* _createNewBallIfNeeded(int start_time) {
	static int last_ball_created_time = 0;
	static int last_treasure_created_time = 0;
	if (last_ball_created_time == 0 || last_ball_created_time < start_time) last_ball_created_time = GetNowCount();
	if (last_treasure_created_time == 0 || last_treasure_created_time < start_time) last_treasure_created_time = GetNowCount();

	// treasure ball creation
	if (last_treasure_created_time < GetNowCount() - TREASURE_INTERVAL_MSEC(start_time)) {
		Ball* new_treasure = newTreasureBall();
		if (new_treasure) new_treasure->imageHandle = treasure_handle;
		last_treasure_created_time = GetNowCount();
		return new_treasure;
	}

	// normal ball(enemy) creation
	if (last_ball_created_time < GetNowCount() - BALL_INTERVAL_MSEC(start_time)) { // interval from the last creation
		Ball* newball = newBall();
		if (newball) newball->imageHandle = (newball->typeId == TYPE_BALL0) ? ball0_handle :
											(newball->typeId == TYPE_BALL1) ? ball1_handle :
											(newball->typeId == TYPE_BALL2) ? ball2_handle :
											(newball->typeId == TYPE_BALL3) ? ball3_handle : -1;
		last_ball_created_time = GetNowCount();
		return newball;
	}
	return NULL;
}

#define RESULT_CONTINUE 0
#define RESULT_EXIT     1
int game() {
	Ball* balls[MAX_BALLS] = { NULL };
	int ball_num = 0;

	int ret = RESULT_EXIT;

	// for fps handling
	int last_frame_time = GetNowCount();

	int start_time = GetNowCount(); // after MAX_SEC elapsed, force finish

	srand((unsigned int) GetNowCount());
	initPlayer();

	char key_state[256];
	while (ProcessMessage() == 0) {
		// get key inputs
		GetHitKeyStateAll(key_state);
		if (key_state[KEY_INPUT_ESCAPE]) {
			ret = RESULT_CONTINUE;
			break; // press Esc to return main menu(launcher())
		}

		// take the player and the balls to the next position
		playerGoNextPosition(key_state);   // the player
		for (int i = 0; i < ball_num; i++) // balls
			balls[i]->goNextPosition();

		// new ball if needed
		Ball* newball;
		if (ball_num + 1 < MAX_BALLS && (newball = _createNewBallIfNeeded(start_time))) {
			balls[ball_num] = newball;
			ball_num++;
		}

		// draw
		ClearDrawScreen();
		drawBalls(balls);
		drawPlayer();
		drawScore();
		drawHearts(heart_handle);
		drawTimeLeft(start_time);

		// fps handling
		while (last_frame_time + 33 > GetNowCount()); // run at 30fps
		last_frame_time = GetNowCount();
		ScreenFlip();

		// finish if needed
		if (_judgeHits(balls)) {
			// you died... too bad
			drawHearts(heart_handle); // update
			blinkDeadPlayer();
			WaitKey();
			ret = RESULT_CONTINUE;
			break;
		} else if (start_time + 1000 * MAX_SEC < GetNowCount()) {
			// time over
			showTimeOver();
			WaitKey();
			ret = RESULT_CONTINUE;
			break;
		}
	}
	_deleteBalls(balls);
	return ret;
}

int launcher() {
	ClearDrawScreen();
	drawInstruction();
	ScreenFlip();
	WaitKey();

	// secret adjusting menu(press F1 to enter, F6 to be harder, F7 to be easier, F10 to exit)
	char key_state[256];
	GetHitKeyStateAll(key_state);
	if (key_state[KEY_INPUT_F1]) {
		DrawFormatString(0, 0, COLOR_MSG, "“ïˆÕ“x’²® : %+d", getBallIntervalAdjustion());
		bool last_f6_pressed = false;
		bool last_f7_pressed = false;
		while (ProcessMessage() == 0) {
			ScreenFlip();
			GetHitKeyStateAll(key_state);
			if (key_state[KEY_INPUT_F6] && !last_f6_pressed) {
				complicate();
				last_f6_pressed = true;
			} else if (key_state[KEY_INPUT_F7] && !last_f7_pressed) {
				uncomplicate();
				last_f7_pressed = true;
			} else if (key_state[KEY_INPUT_F10]) break; // press F10 to exit the menu
			ClearDrawScreen();
			drawInstruction();
			DrawFormatString(0, 0, COLOR_MSG, "“ïˆÕ“x’²® : %+d", getBallIntervalAdjustion());
			if (!key_state[KEY_INPUT_F6]) last_f6_pressed = false;
			if (!key_state[KEY_INPUT_F7]) last_f7_pressed = false;
		}
	}

	return game();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (!initConfig()) error("Failed to init config");
	else if (!_initWindow()) error("Failed to init window");
	else if (!_loadImages()) error("failed to load images");
	else {
		while (1) {
			int result = launcher();
			if (result == RESULT_EXIT) break;
		}
	}
	_freeResources();

	DxLib_End();

	return 0;
}
