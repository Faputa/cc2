//parse and gen

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

int *emit, *e;

void parsegen_init(void) {
	emit = e = (int*)malloc(MAXSIZE * sizeof(int));
}

void parsegen(char *str) {
	//into code
	*e++ = AG; int *_main = e++;
	*e++ = PUSH; *e++ = AX;
	*e++ = CALL; *e++ = 0;
	*e++ = EXIT;
	
	//parse and gen
	tokensrc(str);
	next();
	while(strcmp(tks, "") || tki != -1) {
		declare_glo();
		next();
	}
	*_main = getid("main")->offset;
}
