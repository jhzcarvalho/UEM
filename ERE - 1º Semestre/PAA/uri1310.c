#include<stdio.h>

int memo[100];

int max(int i, int j){
    if (i >=  j){
        return i;
    }else{
        return j;
    }
}

int SomaSubVet(int v[], int n, int tam){
    if (memo[n] != -1000){
        return memo[n];
    }

    if (n == 0){
        memo[n] = v[0];
        return memo[n];
    }else{
        memo[n] = max(SomaSubVet(v, n-1, tam) + v[n],v[n]);

    }

}

int main(){
    int i, tam;
    int soma = -1000;
    printf("escreva o tamanho do vetor\n");
    scanf("%d", &tam);

    for (i = 0; i < tam+10; i++){
        memo[i] = -1000;
    }

    int vet[tam];
    for (i = 0; i < tam; i++){
        printf("insira um elemento na posicao %d\n",i);
        scanf("%d", &vet[i]);
    }

    SomaSubVet(vet, tam, tam);

    for (i=1; i < tam-2;i++){
       if (soma < memo[tam]){
            soma = memo[tam];
        }
    }
    printf("soma maxima %d\n",soma);

    for (i = 0; i < tam+2; i++){
        printf("%d   ",memo[i]);
    }
    return 0;
}
