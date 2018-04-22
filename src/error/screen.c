/* 
	This file is part of 止まれませーん.
	Copyright (c) 2018 QCFium
	All rights reserved
*/

#include "screen.h"

inline void setTerminalColor(WORD color) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, color);
}

inline void resetTerminalColor() {
    setTerminalColor(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
}
