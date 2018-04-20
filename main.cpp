#include <stdlib.h>
#include "DxLib.h"

#include "config.h"
#include "ball.h"
#include "player.h"
#include "draw.h"

int BALL_INTERVAL_MSEC = 700;

bool initWindow() {
	SetDoubleStartValidFlag(TRUE); // allow double starting
	SetOutApplicationLogValidFlag(FALSE); // disable logging into a file
	if (DxLib_Init() == -1) return false;

	if (
		ChangeWindowMode(TRUE) != DX_CHANGESCREEN_OK || // window mode
		SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32) != DX_CHANGESCREEN_OK ||
		SetWindowSize(SCREEN_SIZE_X, SCREEN_SIZE_Y) == -1 ||
		SetWindowSizeChangeEnableFlag(FALSE, FALSE) == -1 ||

		SetDrawScreen(DX_SCREEN_BACK) == -1 || // draw to backside
		SetWindowText(APPLICATION_NAME) != 0)
		return false;
	return true;
}

bool judgeHits(Ball* balls[], int num) {
	for (int i = 0; i < num; i++) {
		if (!balls[i]->inScreen) continue; // the ball has already gone out from the screen

		// get distance of x and that of y
		int distance_x = balls[i]->x - getPlayerX();
		if (distance_x < 0) distance_x = -distance_x;
		int distance_y = balls[i]->y - getPlayerY();
		if (distance_y < 0) distance_y = -distance_y;

		// use Archimedes' theorem(!?) to judge
		int r_sum = PLAYER_CIRCLE_R + balls[i]->circle_r;
		bool ret = (r_sum * r_sum > (distance_x * distance_x) + (distance_y * distance_y));

		if (balls[i]->isTreasure) {
			if (ret) {
				balls[i]->inScreen = false; // it's gotten
				increasePlayerScore();
				return false; // it's not game over
			}
		} else {
			if (ret) {
				bool died = killPlayer();
				if (!died) balls[i]->inScreen = false;
				return died;
			}
		}
	}
	return false;
}

void freeResources(Ball* balls[]) {
	for (int i = 0; i < MAX_BALLS; i++) { // free(NULL); does nothing
		delete(balls[i]);
	}
}

Ball* createNewBallIfNeeded() {
	static int last_ball_created_time = 0;
	static int last_treasure_crated_time = 0;
	if (last_ball_created_time == 0) last_ball_created_time = GetNowCount();
	if (last_treasure_crated_time == 0) last_treasure_crated_time = GetNowCount();

	// treasure ball creation
	if (last_treasure_crated_time < GetNowCount() - TREASURE_INTERVAL_MSEC) {
		Ball* new_treasure = newTreasureBall();
		last_treasure_crated_time = GetNowCount();
		return new_treasure;
	}

	// normal ball(enemy) creation
	if (last_ball_created_time < GetNowCount() - BALL_INTERVAL_MSEC) { // interval from the last creation
		Ball* newball = newBall();
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
	int heart_handle = LoadGraph("heart.png");
	if (heart_handle == -1) return ret;

	// for fps handling
	int last_frame_time = GetNowCount();

	int start_time = GetNowCount(); // after MAX_SEC elapsed, force finish

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
		if (ball_num + 1 < MAX_BALLS && (newball = createNewBallIfNeeded())) {
			balls[ball_num] = newball;
			ball_num++;
		}

		// fps handling
		while (last_frame_time + 33 > GetNowCount()); // run at 30fps
		last_frame_time = GetNowCount();

		// draw
		ClearDrawScreen();
		drawBalls(balls, ball_num);
		drawPlayer();
		drawScore();
		drawHearts(heart_handle);
		drawTimeLeft(start_time);
		ScreenFlip();

		// finish if needed
		if (judgeHits(balls, ball_num)) {
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
	freeResources(balls);
	return ret;
}

int launcher() {
	ClearDrawScreen();
	drawInstruction();
	ScreenFlip();
	WaitKey();

	// secret adjusting menu(press F1 to enter, F6 to be easier, F7 to be harder, F10 to exit)
	char key_state[256];
	GetHitKeyStateAll(key_state);
	if (key_state[KEY_INPUT_F1]) {
		DrawFormatString(0, 0, COLOR_MSG, "Entering configuration : %d", BALL_INTERVAL_MSEC);
		ScreenFlip();
		bool last_f6_pressed = false;
		bool last_f7_pressed = false;
		while (ProcessMessage() == 0) {
			if (key_state[KEY_INPUT_F6] && !last_f6_pressed) {
				BALL_INTERVAL_MSEC += 50;
				ClearDrawScreen();
				drawInstruction();
				DrawFormatString(0, 0, GetColor(0, 0, 255), "Entering configuration : %d", BALL_INTERVAL_MSEC);
				ScreenFlip();
				last_f6_pressed = true;
			} else if (key_state[KEY_INPUT_F7] && !last_f7_pressed) {
				BALL_INTERVAL_MSEC -= 50;
				ClearDrawScreen();
				drawInstruction();
				DrawFormatString(0, 0, GetColor(255, 0, 0), "Entering configuration : %d", BALL_INTERVAL_MSEC);
				ScreenFlip();
				last_f7_pressed = true;
			} else if (key_state[KEY_INPUT_F10]) break;
			if (!key_state[KEY_INPUT_F6]) last_f6_pressed = false;
			if (!key_state[KEY_INPUT_F7]) last_f7_pressed = false;
		
			GetHitKeyStateAll(key_state);
		}
	}

	return game();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (!initWindow()) {
		DxLib_End();
		return 1;
	};

	while (1) {
		int result = launcher();
		if (result == RESULT_EXIT) break;
	}

	DxLib_End();

	return 0;
}
