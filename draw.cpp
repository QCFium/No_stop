#include <stdlib.h>
#include "DxLib.h"

#include "draw.h"
#include "ball.h"
#include "player.h"
#include "config.h"

#define strlen_mb _mbstrlen

#define RESULT_RESULT_POSITION_Y (SCREEN_SIZE_Y - 140)
#define RESULT_SCORE_POSITION_Y  (SCREEN_SIZE_Y - 100)
#define RESULT_ANYKEY_POSITION_Y (SCREEN_SIZE_Y - 60)


void waitMsec(int msec) {
	if (msec <= 0) return;
	int start_time = GetNowCount();
	while (ProcessMessage() == 0 && start_time + msec > GetNowCount());
}

// draw given balls to the screen
void drawBalls(Ball* balls[]) {
	for (int i = 0; i < MAX_BALLS; i++) {
		if (!balls[i]) return; // no more ball
		if (!balls[i]->inScreen) continue;
		if (balls[i]->inScreen) // if it's still in the window...
			DrawCircle(balls[i]->x, balls[i]->y, balls[i]->circle_r,
				balls[i]->color, 32);
	}
}

void drawPlayer() {
	int time_from_last = getTimeElapsedFromLastDamage();
	unsigned int color = COLOR_PLAYER;
	if (time_from_last != -1 && time_from_last < DEAD_PLAYER_BLINK_INTERVAL_MSEC * DEAD_PLAYER_BLINK_NUM) {
		if (!((time_from_last / DEAD_PLAYER_BLINK_INTERVAL_MSEC) % 2)) color = COLOR_PLAYER_HIT;
	}

	DrawCircle(getPlayerX(), getPlayerY(), PLAYER_CIRCLE_R, color, 32);
}

void drawHearts(int heart_handle) {
	int lives = getLifeNumLeft();
	if (!lives) { // no more lives, erase all hearts
		DrawBox(0, 30, 30 * 4, 60, COLOR_BACKGROUND, TRUE);
	} else {
		for (int i = 0; i < lives; i++) {
			DrawGraph(i * 30, 30, heart_handle, TRUE);
		}
	}
}

// draw score on playing screen
void drawScore() {
	static int last_score = 0;
	static int last_increase_time = 0;

	if (getPlayerScore() < last_score) {
		last_score = 0;
		last_increase_time = 0;
	} else if (getPlayerScore() > last_score) {
		last_increase_time = GetNowCount();
		last_score = getPlayerScore();
	}

	bool increasing = (last_increase_time + SCORE_BLINK_INTERVAL_MSEC * SCORE_BLINK_NUM > GetNowCount());
	bool show = !increasing || ((GetNowCount() - last_increase_time) / SCORE_BLINK_INTERVAL_MSEC) % 2;

	int font = GetDefaultFontHandle();
	int size;
	GetFontStateToHandle(NULL, &size, NULL, font);

	char score_str[64];
	snprintf(score_str, 63, "%s%d", SCORE_STR, getPlayerScore());

	int width = GetDrawStringWidth(score_str, strlen_mb(score_str));
	DrawString(0, 0, score_str, show ? COLOR_MSG : COLOR_SCORE_INCREASING);
}

void drawTimeLeft(int start_time) {
	int time_elapsed_msec = GetNowCount() - start_time;
	int sec_left = MAX_SEC - time_elapsed_msec/1000;

	char time_str[16];
	snprintf(time_str, 15, "残り%d秒", sec_left);
	int width = GetDrawStringWidth(time_str, strlen_mb(time_str));

	DrawString(SCREEN_SIZE_X - width, 0, time_str, sec_left < 10 ? COLOR_LOW_TIME : COLOR_MSG);
}

// draw score on result screen
void _drawScoreResult() {
	char score_str[64];
	snprintf(score_str, 63, "%s%d", SCORE_STR, getPlayerScore());
	
	int width = GetDrawStringWidth(score_str, strlen_mb(score_str));
	DrawString((SCREEN_SIZE_X - width) / 2, RESULT_SCORE_POSITION_Y, score_str, COLOR_MSG);
}

// draw that press any key on result screen
void _drawAnykeyMessage() {
	int anykey_str_width = GetDrawStringWidth(ANYKEY_STR, strlen_mb(ANYKEY_STR));
	DrawString((SCREEN_SIZE_X - anykey_str_width) / 2, RESULT_ANYKEY_POSITION_Y, ANYKEY_STR, COLOR_MSG);
}

void blinkDeadPlayer() {
	bool hit_color_used = true;
	int blink_count = 0;

	while (ProcessMessage() == 0) {
		DrawCircle(getPlayerX(), getPlayerY(), PLAYER_CIRCLE_R, hit_color_used ? COLOR_PLAYER_HIT : COLOR_PLAYER, 32);
		ScreenFlip();

		if (++blink_count >= DEAD_PLAYER_BLINK_NUM) break;
		hit_color_used = !hit_color_used;
		waitMsec(DEAD_PLAYER_BLINK_INTERVAL_MSEC);
	}

	// draw that it's over
	int gameover_str_width = GetDrawStringWidth(GAMEOVER_STR, strlen_mb(GAMEOVER_STR));
	DrawString((SCREEN_SIZE_X - gameover_str_width)/2, RESULT_RESULT_POSITION_Y, GAMEOVER_STR, COLOR_MSG);

	_drawScoreResult();
	ScreenFlip();
	Sleep(WAIT_AFTER_SHOW_RESULT_MSEC);
	_drawAnykeyMessage();
	ScreenFlip();
}

void showTimeOver() {
	// draw that it's over
	int gameover_str_width = GetDrawFormatStringWidth(TIMEOVER_STR, MAX_SEC);
	DrawFormatString((SCREEN_SIZE_X - gameover_str_width) / 2, RESULT_RESULT_POSITION_Y, COLOR_MSG, TIMEOVER_STR, MAX_SEC);

	_drawScoreResult();
	ScreenFlip();
	Sleep(WAIT_AFTER_SHOW_RESULT_MSEC);
	_drawAnykeyMessage();
	ScreenFlip();
}

void drawInstruction() {
	const char* instruction_str1 = "矢印キー(上下左右)で画面上の白い球を操作してください。";
	const char* instruction_str2 = "緑色の玉に触れると%dポイント加算されます。";
	const char* instruction_str3 = "それ以外の色の玉に触れると残りのライフが減ってしまいます。";
	const char* instruction_str4 = "残りライフが0になるとゲームオーバーです。";
	const char* instruction_str5 = "開始から%d秒経っても終わりです。";
	const char* instruction_str6 = "つまり%d秒以内に他のボールに当たらずに何個緑を回収できるかが勝負です";

	int width = GetDrawFormatStringWidth(instruction_str1); // hacky(only checking about str1)
	int str_x = (SCREEN_SIZE_X - width) / 2;
	int str_y = 80; // hacky
		
	DrawFormatString(str_x, str_y, COLOR_MSG, instruction_str1);
	DrawFormatString(str_x, str_y + 20, GetColor(0, 255, 0), instruction_str2, POINTS_PER_CATCH); // green
	DrawFormatString(str_x, str_y + 40, GetColor(255, 0, 0), instruction_str3); // red
	DrawFormatString(str_x, str_y + 60, COLOR_MSG, instruction_str4);
	DrawFormatString(str_x, str_y + 80, COLOR_MSG, instruction_str5, MAX_SEC);
	DrawFormatString(str_x, str_y + 100, GetColor(255, 255, 0), instruction_str6, MAX_SEC);
	_drawAnykeyMessage();
}
