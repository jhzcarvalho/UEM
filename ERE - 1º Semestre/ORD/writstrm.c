/* cria arquivo de nomes e endereços organizado
como uma sequência (stream) de bytes.
Os campos são delimitados por | */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DELIM_STR "|"

int input(char *str, int size);

int main() {
    char first[31], last[31], address[31], city[31], state[16], zip[9];
    char filename[21];
    int size_last;
    FILE* fd;

    printf("Entre com o nome do arquivo a ser criado: ");
    input(filename, 20);

    if ((fd = fopen(filename, "w")) == NULL) {
        printf("Erro na criação do arquivo --- programa abortado.\n");
        exit(1);
    }

    printf("\n\nDigite o Sobrenome, ou <ENTER> para sair\n>>> ");
    size_last = input(last, 30);

    while(size_last > 0) {
        printf("\nPrimeiro nome\n>>> ");
        input(first, 30);
        printf("\nEndereco\n>>> ");
        input(address, 30);
        printf("\nCidade\n>>> ");
        input(city, 30);
        printf("\nEstado\n>>> ");
        input(state, 15);
        printf("\nCEP\n>>> ");
        input(zip, 8);

        /* envia as strings para o arquivo */
        fputs(last, fd);
        fputs(DELIM_STR, fd);
        fputs(first, fd);
        fputs(DELIM_STR, fd);
        fputs(address, fd);
        fputs(DELIM_STR, fd);
        fputs(city, fd);
        fputs(DELIM_STR, fd);
        fputs(state, fd);
        fputs(DELIM_STR, fd);
        fputs(zip, fd);
        fputs(DELIM_STR, fd);

        /* começa a próxima entrada */
        printf("\n\nDigite o Sobrenome, ou <ENTER> para sair\n>>> ");
        size_last = input(last, 30);
    }

    /* fechar o arquivo */
    fclose(fd);

    printf("Programa finalizado.\n");

    return EXIT_SUCCESS;
}

/* função que lê uma linha do stdin para a string str */
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
