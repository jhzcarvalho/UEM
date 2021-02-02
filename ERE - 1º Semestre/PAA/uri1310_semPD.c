#include<stdio.h>
int SomaSubVet(int v[], int tam){
    int i;
    int max1 = 0;
    int max2 = 0;

    for (i = 0; i < size; i++){
        max2 += v[i];

        if (max2 < 0){
            max2 = 0;
        }
        if (max1 < max2){
            max1 = max2;
        }
    }
    return max1;
}
int main{
    int i, tam;
    int soma_max = 0;
    printf("escrva o tamanho do vetor\n");
    scanf("%d", &tam);

    int vet[tam];
    for (i = 0; i < size; i++){
        printf("insira um elemento na posicao %d\n",i);
        scanf("%d", &vet[i]);
    }

    soma_max = SomaSubVet(vet, tam);
    printf("soma maxima %d\n",soma_max);
    return 0;
}
