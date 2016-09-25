#include <stdio.h>
#include "test.h"

void endl() {
	printf("\n");
}
void space() {
	printf(" ");
}
int scan() {
	int x;
	scanf("%d", &x);
	return x;
}
void print(int x) {
	printf("%d", x);
}