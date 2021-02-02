#include <stdio.h>

int fib( int n ) {
    if (n < 2){
        return 1;
    }else {
        return (fib(n-1) + fib(n-2));
    }
}

int main() {
    int n, result;
    scanf("%d\n", &n);

    result = fib(n);
    printf("fibonacci: %d", result);


    return 0;
}
