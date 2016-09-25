#include <stdio.h>
#include "test.h"

static char strbuf[BUFSIZE];

void printint(int i) {
	printf("%d", i);
}
void printchar(char c) {
	printf("%c", c);
}
void printstr(char *s) {
	printf("%s", s);
}
int scanint(void) {
	int i;
	fflush(stdin);
	scanf("%d", &i);
	return i;
}
char scanchar(void) {
	char c;
	fflush(stdin);
	scanf("%c", &c);
	return c;
}
char* scanstr(void) {
	fflush(stdin);
	scanf("%s", strbuf);
	return strbuf;
}