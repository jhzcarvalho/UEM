#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TAM 100

int input(char *str, int size) {
    int i = 0;
    char c;
    while ((c = getchar()) != '\n') {
        if (size > 0) {
            str[i] = c;
            i++;
            size--;
        }
    }
    str[i] = '\0';
    return i;
}


int main(void) {
    char filename[MAX_TAM];
    char buffer[MAX_TAM];
    int i;
    printf("insira a operacao a ser feita:(i|e)\n");
    i = input(buffer, MAX_TAM);

    if (strcmp(buffer, "i") == 1){
        printf("insira o arquivo a ser lido\n");
        i = input(filename, MAX_TAM);

        FILE *inputFile;
        if ((inputFile = fopen(filename, "r")) == NULL) {
            printf("Erro na abertura do arquivo --- programa abortado.\n");
            exit(1);
        }else{
            printf("%s aberto com sucesso\n", buffer);
        }
        

    }

    if (strcmp(buffer, "e") == 1){
        printf("insira o arquivo a ser editado\n");
        i = input(filename, MAX_TAM);

        FILE *inputFile;
        if ((inputFile = fopen(filename, "r")) == NULL) {
            printf("Erro na abertura do arquivo --- programa abortado.\n");
            exit(1);
        }else{
            printf("%s aberto com sucesso\n", buffer);
        }

    }
    return 0;
}
