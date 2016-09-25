#include "test.h"

int add(int a, int b) {
	return a + b;
}
int sub(int a, int b) {
	return a - b;
}
int mul(int a, int b) {
	return a * b;
}
int div(int a, int b) {
	return a / b;
}
int main() {
	int a = scanint();
	int b = scanint();
	printint(add(a, b)); printstr("\n");
	printint(sub(a, b)); printstr("\n");
	printint(mul(a, b)); printstr("\n");
	printint(div(a, b)); printstr("\n");
}