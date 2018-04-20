#pragma once

#include "stdio.h"

int getPlayerX();
int getPlayerY();
int getPlayerScore();
void increasePlayerScore();

/* initialize player's state */
void initPlayer();

bool killPlayer();

int getLifeNumLeft();

int getTimeElapsedFromLastDamage();

/* handle key input and go it to the next position */
void playerGoNextPosition(char* key_state);
