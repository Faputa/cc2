//main

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static void print() {
	int i = api_getarg(1);
	printf("%d", i);
}

static void scan() {
	int i;
	scanf("%d", &i);
	api_return(i);
}

static void endl() {
	printf("\n");
}

static void space() {
	printf(" ");
}

int main(int argc, char *argv[]) {
	int src = 0, debug = 0;
	char *fname = NULL;
	FILE *fp;
	if(argc < 2) { printf("error2!\n"); exit(-1); }
	for(int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-s")) {
			src = 1;
		} else if(!strcmp(argv[i], "-d")) {
			debug = 1;
		} else {
			char *j;
			if((j = strrchr(argv[i], '.'))) {
				if(!strcmp(j, ".c")) fname = argv[i];
				else { printf("error3!\n"); exit(-1); }
			} else { printf("error4!\n"); exit(-1); }
		}
	}
	if(!fname) { printf("error5!\n"); exit(-1); }
	if(!(fp = fopen(fname, "r"))) { printf("error6!\n"); exit(-1); }
	
	declare_init();
	token_init();
	id_init();
	expr_init();
	vm_init();
	api_init();
	
	{ int i = fread(p, sizeof(char), MAXSIZE, fp); p[i] = '\0'; }//printf("%s",p);
	fclose(fp);
	
	//reg api
	api_register(print, "int print(int x);");
	api_register(scan, "int scan();");
	api_register(endl, "int endl();");
	api_register(space, "int space();");
	
	//into code
	*e++ = AG; int *_main = e++;
	*e++ = PUSH; *e++ = AX;
	*e++ = SET; *e++ = AX; int *_exit = e++;
	*e++ = PUSH; *e++ = AX;
	*e++ = CALL; *e++ = 0;//*e++ = JMP; int *_main = e++;
	*_exit = e - emit; *e++ = EXIT;
	
	//parse
	next();
	while(strcmp(tks, "") || tki != -1) {
		declare(GLO);
		next();
	}
	*_main = getid("main") -> offset;
	
	//run
	run_vm(src, debug);
	return 0;
}
