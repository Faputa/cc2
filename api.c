//外部接口

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static Api *api, *apis;

void api_init(void) {
	static int is_init = 0;
	if(!is_init) {
		apis = api = (Api*)malloc(MAXSIZE * sizeof(Api));
		is_init = 1;
	}
}

void api_register(Api fun, char *proto) {
	tokensrc(proto);
	next();
	Id *this_id = gid;
	declare(GLO);
	if(this_id -> type -> base != FUN) { printf("error1!\n"); exit(-1); }
	this_id -> type -> base = API;
	this_id -> offset = api - apis;
	*api++ = fun;
}

void api_call(int offset) {
	apis[offset]();
}

int api_getint(int index) { //index == n 表示第n个参数
	return data[data[SP] - index];
}

char api_getchar(int index) {
	return data[data[SP] - index];
}

char* api_getstr(int index) { //返回一个临时的字符串，必须及时保存
	int stroff = data[data[SP] - index];
	int *str = data + stroff;
	int i = 0;
	static char strbuf[BUFSIZE];
	while(str[i]) {
		strbuf[i] = str[i];
		i++;
	}
	strbuf[i] = '\0';
	return strbuf;
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
