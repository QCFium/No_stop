@echo off
gcc main.c screen.c -I. -O3 -flto -ffunction-sections -fdata-sections -Wall -Wno-format-truncation -o ../../error.exe
pause

