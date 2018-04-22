/* 
	This file is part of 止まれませーん.
	Copyright (c) 2018 QCFium
	All rights reserved
*/

#pragma once

#include <windows.h>
#include <conio.h>

#define TXT_R FOREGROUND_RED
#define TXT_G FOREGROUND_GREEN
#define TXT_B FOREGROUND_BLUE
#define TXT_BRIGHT FOREGROUND_INTENSITY

#define BG_R BACKGROUND_RED
#define BG_G BACKGROUND_GREEN
#define BG_B BACKGROUND_BLUE
#define BG_BRIGHT BACKGROUND_BRIGHT

void setTerminalColor(WORD color);
/** 
	Set terminal color to specified color

	WORD color :
		FOREGROUND_RED
		FOREGROUND_GREEN
		FOREGROUND_BLUE
		FOREGROUND_INTENSITY
		
		BACKGROUND_RED
		BACKGROUND_GREEN
		BACKGROUND_BLUE
		BACKGROUND_INTENSITY
*/

void resetTerminalColor();
/**
	Reset the terminal color
	Actually, set these colors:
	
	FOREGROUND_BLUE
	FOREGROUND_GREEN
	FOREGROUND_RED
*/
