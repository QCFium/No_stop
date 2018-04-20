#pragma once

#include "ball.h"

/* draw given balls to the screen */
void drawBalls(Ball* balls[]);

/* show blinking animation for dead player */
void blinkDeadPlayer();

/* show player */
void drawPlayer();

/* show hearts depending on the player lives left */
void drawHearts(int heart_handle);

/* show score string on top left */
void drawScore();

void drawTimeLeft(int start_time);

/* show that time is over */
void showTimeOver();


void drawInstruction();
