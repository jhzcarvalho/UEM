#include <cstdio>

int main() {

	int days;
	while (scanf("%d", &days) == 1) {

	int cost;
	scanf("%d", &cost);

	int rev[days];

	for (int i = 0; i < days; ++i) scanf("%d", &rev[i]);
	for (int i = 0; i < days; ++i) rev[i] -= cost;

	int somas[days];
	int soma = 0;

	for (int i = 0; i < days; ++i) {
		soma += rev[i];
		somas[i] = soma;
	}

	int index_max = 0;
	for (int i = 1; i < days; i++) {
		if (somas[i] > somas[index_max])
			index_max = i;
	}

	soma = 0;
	for (int i = index_max; i >=0; i--) {
		soma += rev[i];
		somas[i] = soma;
	}

	int index_min = 0;
	for (int i = 1; i < index_max; i++) {
		if (somas[i] > somas[index_min])
			index_min = i;
	}

	if (somas[index_min] < 0) somas[index_min] = 0;

	printf("%d\n", somas[index_min]);

	}

	return 0;
}
