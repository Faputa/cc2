#include "test.h"

int f(int a, int b, int c, int d, int e, int f) {
	printint(a); printstr("\n");
	printint(b); printstr("\n");
	printint(c); printstr("\n");
	printint(d); printstr("\n");
	printint(e); printstr("\n");
	printint(f); printstr("\n");
	int g[100] = {0, 700};
	int h = 800;
	printint(g[1]); printstr("\n");
	printint(h); printstr("\n");
}

int main() {
	f(100, 200, 300, 400); printstr("----\n");
	f(100, 200, 300, 400, 500, 600); printstr("----\n");
	f(100, 200, 300, 400, 500, 600, 10, 20);
}