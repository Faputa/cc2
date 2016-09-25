//外部接口

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static Api *api, *apis;
static char *strbuf;
static int si;

void api_init(void) {
	static int is_init = 0;
	if(!is_init) {
		apis = api = (Api*)malloc(MAXSIZE * sizeof(Api));
		strbuf = (char*)malloc(MAXSIZE * sizeof(char));
		si = 0;
		is_init = 1;
	}
}

void api_register(Api fun, char *proto) {
	char *_p = p;
	p = proto;
	next();
	Id *this_id = gid;
	declare(GLO);
	if(this_id -> type -> base != FUN) { printf("error1!\n"); exit(-1); }
	this_id -> type -> base = API;
	this_id -> offset = api - apis;
	*api++ = fun;
	p = _p;
}

void api_call(int offset) {
	apis[offset]();
}

int api_getint(int index) { //index == n 表示第n个参数
	return data[data[SP] - index];
}

char api_getchar(int index) {
	//printf("-- %d --\n",data[data[SP] - index]);
	return data[data[SP] - index];
}

char* api_getstr(int index) {
	int stroff = data[data[SP] - index];
	int *str = data + stroff;
	int _si = si;
	for(int i = 0; str[i] != '\0'; i++) {
		strbuf[si++] = str[i];
		if(si == MAXSIZE - 1) {
			str[si] = '\0';
			si = 0;
		}
	}
	strbuf[si++] = '\0';
	return strbuf + _si;
}

void api_setint(int i) {
	data[AX] = i;
}

void api_setchar(char c) {
	data[AX] = c;
}

void api_setstr(char *s) {
	data[AX] = sgetstr(s) -> offset;
}
