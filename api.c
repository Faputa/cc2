//外部接口

#include "cc.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static Api *api, *apis;
static char *strbuf;

void api_init(void) {
	apis = api = (Api*)malloc(MAXSIZE * sizeof(Api));
	strbuf = (char*)malloc(MAXSIZE * sizeof(char));
}

void api_register(Api fun, char *proto) {
	p = proto;
	next();
	Id *id = gid;
	declare_glo();
	if(id->type->tykind != FUN) error("line %d: error!\n", line);
	id->type->tykind = API;
	id->offset = api - apis;
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
	int *str = data + data[data[SP] - index];
	int i = 0;
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
	data[AX] = sgetstr(s)->offset;
}
