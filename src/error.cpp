#include "error.h"
#include "stdlib.h"
#include "stdio.h"

void error(const char* msg) {
	char errmsg[256];
	snprintf(errmsg, 255, "start echo off^&error %s", msg);
	system(errmsg);
}
