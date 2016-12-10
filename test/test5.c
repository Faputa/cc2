#include "test.h"

int f(int a[], int b) {
	return a[0] + b;
}
int main() {
	int a[2*5][10] = {{},{100,200,300}};
	printint(a[1][2]);
	printstr("\n");
	int *b = &a[1][1];
	printint(*b);
	printstr("\n");
	int (*c)[10] = &a[1];
	printint((*c)[0]);
	printstr("\n");
	printint(*(*c + 0));
	printstr("\n");
	int (*d)(int a[], int b) = &f;
	printint((*d)(a[1], 25));
	printstr("\n");
	printint(d(a[1], 25));
}
