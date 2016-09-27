#include "test.h"

int main() {
	for(int i = 1; i <= 9; i = i + 1) {
		for(int j = 1; j <= i; j = j + 1) {
			printint(j); printstr("x"); printint(i); printstr("="); printint(i * j); printstr("\t");
		}
		printstr("\n");
	}
}