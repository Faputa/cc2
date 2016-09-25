#include "test.h"

int factoral(int n) {
	if(n == 0) {
		return 1;
	} else {
		return n * factoral(n-1);
	}
}
int factoral_(int n) {
	if(n == 0) {
		return 1;
	} else {
		int tem = 1;
		for(int i = 1; i <= n; i = i + 1) {
			tem = tem * i;
		}
		return tem;
	}
}
int fibonacci(int n) {
	if(n <= 0) {
		return 1;
	} else if(n == 1) {
		return 1;
	} else {
		return fibonacci(n - 1) + fibonacci(n - 2);
	}
}
int fibonacci_(int n) {
	if(!n > 0) {
		return 1;
	} else if(n == 1) {
		return 1;
	} else {
		int a1 = 1;
		int a2 = 1;
		for(int i = 2; i <= n; i = i + 1) {
			a2 = a1 + a2;
			a1 = a2 - a1;
		}
		return a2;
	}
}
int ackerman(int n, int m) { //艾可门函数 http://t.cn/RtYNtpn
	if(n == 1 && m == 0) {
		return 2;
	} else if(n == 0 && m >= 0) {
		return 1;
	} else if(n >= 2 && m == 0) {
		return n + 2;
	} else if(n >= 1 && m >= 1) {
		return ackerman(ackerman(n - 1, m), m - 1);
	}
}
int ackermann(int m, int n) { //阿克曼函数 http://t.cn/RtYNS9B
	if(m == 0) {
		return n + 1;
	} else if(m > 0 && n == 0) {
		return ackermann(m - 1, 1);
	} else if(m > 0 && n > 0) {
		return ackermann(m - 1, ackermann(m, n - 1));
	}
}
int main() {
	printint(factoral(5)); printstr("\n");
	printint(factoral_(5)); printstr("\n");
	printint(fibonacci(5)); printstr("\n");
	printint(fibonacci_(5)); printstr("\n");
	printint(ackerman(4,2)); printstr("\n");
	printint(ackermann(3,7)); printstr("\n");
}