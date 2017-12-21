//main

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static void printint(void) {
	int i = api_getint(1);
	printf("%d", i);
}

static void printchar(void) {
	char c = api_getchar(1);
	printf("%c", c);
}

static void printstr(void) {
	char *s = api_getstr(1);
	printf("%s", s);
}

static void scanint(void) {
	int i;
	fflush(stdin);
	scanf("%d", &i);
	api_setint(i);
}

static void scanchar(void) {
	char c;
	fflush(stdin);
	scanf("%c", &c);
	api_setchar(c);
}

static void scanstr(void) {
	char s[BUFSIZE];
	fflush(stdin);
	scanf("%s", s);
	api_setstr(s);
}

int main(int argc, char *argv[]) {
	int src = 0, debug = 0;
	char *fname = NULL;
	FILE *fp;
	if(argc < 2) error("line %d: error!\n", line);
	for(int i = 1; i < argc; i++) {
		if(!strcmp(argv[i], "-s")) {
			src = 1;
		} else if(!strcmp(argv[i], "-d")) {
			debug = 1;
		} else {
			char *j;
			if((j = strrchr(argv[i], '.'))) {
				if(!strcmp(j, ".c")) fname = argv[i];
				else error("line %d: error!\n", line);
			} else error("line %d: error!\n", line);
		}
	}
	if(!fname) error("line %d: error!\n", line);
	if(!(fp = fopen(fname, "r"))) error("line %d: error!\n", line);
	char *p = (char*)malloc(MAXSIZE * sizeof(char));
	{ int i = fread(p, sizeof(char), MAXSIZE, fp); p[i] = '\0'; }//printf("%s",p);
	fclose(fp);
	
	//init
	vm_init();
	api_init();
	type_init();
	token_init();
	ident_init();
	declare_init();
	parsegen_init();
	
	//reg api
	api_register(printint, "void printint(int i);");
	api_register(printchar, "void printchar(char c);");
	api_register(printstr, "void printstr(char *s);");
	api_register(scanint, "int scanint();");
	api_register(scanchar, "char scanchar();");
	api_register(scanstr, "char* scanstr();");
	
	//parse and gen
	parsegen(p);
	
	//run
	vm_run(src, debug);
	return 0;
}
