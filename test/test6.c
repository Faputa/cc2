#include "test.h"

int main() {
	int a = 100;
	char *b = "200";
	char c[4] = "300";
	int d[10] = {400, 500};
	int e = 600;
	char f = '7', g = '0', h = '0';
	printint(a); printstr("\n");
	printstr(b); printstr("\n");
	printstr(c); printstr("\n");
	printint(d[0]); printstr("\n");
	printint(d[1]); printstr("\n");
	printint(e); printstr("\n");
	printchar(f); printchar(g); printchar(h);
}