#include <stdio.h>

int main() {
	int i;
	for (i = 1; i < 100; i++) {
		if ((i % 3 == 0) || (i % 5 == 0)) {
			if (i % 3 == 0) {
				printf("fizz");
			}
			if (i % 5 == 0) {
				printf("buzz");
			}
			printf("\n");
		}
		else {
			printf("%d\n", i);
		}
	}
}