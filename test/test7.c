#include "test.h"

int main() {
	int i = scanint();
	printint(i); printstr("\n");
	char c = scanchar();
	printchar(c); printstr("\n");
	char *s = scanstr();
	printstr(s);
}