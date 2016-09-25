#include "test.h"

int main() {
	int a = 100;
	char *b = "200";//char *b = "200";
	int c[10] = {300, 400};
	int d = 500;
	char e = '6';
	printint(a); endl();
	printstr(b); endl();
	printint(c[0]); endl();
	printint(c[1]); endl();
	printint(d); endl();
	printchar(e); endl();
}