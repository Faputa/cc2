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
	space(); printint(add(a, b)); endl();
	space(); printint(sub(a, b)); endl();
	space(); printint(mul(a, b)); endl();
	space(); printint(div(a, b)); endl();
}