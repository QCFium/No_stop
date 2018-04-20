#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "screen.h"
#include "../../src/config.h"

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		setTerminalColor(TXT_BRIGHT | TXT_R);
		printf("No arguments");
		resetTerminalColor();
		return 1;
	}
	
	printf(APPLICATION_NAME " : ");
	setTerminalColor(TXT_BRIGHT | TXT_R);
	printf("\nFATAL : %s", argv[1]);
	for (int i = 3; i <= argc; i++) {
		printf(" %s", argv[i-1]);
	}
	resetTerminalColor();
	while(1);
	
	return 0;
}
