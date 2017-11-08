#include "cc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void error(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	exit(0);
}

void warning(char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}
