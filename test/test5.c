#include "test.h"

int f(int a[], int b) {
	return a[0] + b;
}
int main() {
	int a[10][10] = {{},{100,200,300}};
	printint(a[1][2]);
	endl();
	int *b = &a[1][1];
	printint(*b);
	endl();
	int (*c)[10] = &a[1];
	printint((*c)[0]);
	endl();
	printint(*(*c + 0));
	endl();
	int (*d)(int a[], int b) = &f;
	printint((*d)(a[1], 25));
	endl();
	printint(d(a[1], 25));
}