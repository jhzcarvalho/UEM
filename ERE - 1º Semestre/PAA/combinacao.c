#include <stdio.h>

int memo[20][20] = {-1};

int comb(int n, int k){
    int ans;
    if (memo[n][k] == -1){
        if (k == 0 || n == k){
            ans = 1;
        }else{
            ans = comb(n-1,k) + comb(n-1,k-1);
            memo[n][k] = ans;
        }
    }
    return memo[n][k];
}

int main (){

    int n,k,resp,i,j;

    printf ("insira um valor de N\n");
    scanf("%d", &n);

    printf ("insira um valor de K\n");
    scanf("%d", &k);

    for (i = 0 ; i < n ; i++){
        for (j = 0 ; j < k ; j++){
            memo[i][j] = -1;
        }
    }

    for (i = 0 ; i < n ; i++){
        for (j = 0 ; j < k ; j++){
            printf(" %d ", memo[i][j]);
        }
        printf ("\n");
    }

    resp = comb(n,k);
    for (i = 0 ; i < n ; i++){
        for (j = 0 ; j < k ; j++){
            printf(" %d ", memo[i][j]);
        }
        printf ("\n");
    }
    printf ("A combinação de %d elementos %d a %d eh: %d\n",n,k,k,resp);
}
