#include <iostream>
#include <limits>

int opt_pd(int *blocks, int n, int len) {

    if (!len) return 0;

    if (n < 0) return std::numeric_limits<int>::max();

    int count = 0;

    if (blocks[n] <= len)
        count = len/blocks[n];

    int new_len = len - count*blocks[n];

    return std::min(opt_pd(blocks, n-1, len), (opt_pd(blocks, n-1, new_len) + count));
}

int main() {
    int tests;
    std::cin >> tests;

    while (tests--) {
        int n, len;
        std::cin >> n >> len;

        int *blocks;
        blocks = new int[n];

        for(int i = 0; i < n; i++)
            std::cin >> blocks[i];

        std::cout << opt_pd(blocks, n-1, len) << std::endl;

        delete[] blocks;
    }

    return 0;
}
