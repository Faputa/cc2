#include "test.h"

int main() { //10亿次循环
	int i = 0;
	int result = 0;
	while(i < 1000) {
		int j = 0;
		while(j < 1000) {
			int k = 0;
			while(k < 1000) {
				k = k + 1;
				result = result + 1;
			}
			j = j + 1;
		}
		i = i + 1;
	}
	printint(result);
}