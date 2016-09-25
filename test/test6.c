#include "test.h"

int main() {
	int a = 100;
	char *b = "200";//char *b = "200";
	int c[10] = {300, 400};
	int d = 500;
	char e = '6';
	printint(a); printstr("\n");
	printstr(b); printstr("\n");
	printint(c[0]); printstr("\n");
	printint(c[1]); printstr("\n");
	printint(d); printstr("\n");
	printchar(e); printstr("\n");
}