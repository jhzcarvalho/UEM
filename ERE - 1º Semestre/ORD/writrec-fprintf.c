#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REC_SIZE 512
#define DELIM_STR "|"

char *prompt[] = {
    "Digite o Sobrenome, ou <ENTER> para sair: ",
    "                           Primeiro nome: ",
    "                                Endereco: ",
    "                                  Cidade: ",
    "                                  Estado: ",
    "                                     CEP: ",
    "" // string vazia para encerrar o loop
};

int input(char *str, int size);
void append_field(char *buffer, char *field);

int main() {
    char recbuff[MAX_REC_SIZE + 1];
    char response[51];
    char filename[21];
    int i;
    int resp_lgth;
    int rec_lgth;
    FILE *fd;

    printf("Entre com o nome do arquivo a ser criado: ");
    input(filename, 20);

    if ((fd = fopen(filename, "w")) == NULL) {
        printf("Erro na criação do arquivo --- programa abortado.\n");
        exit(1);
    }

    printf("\n\n%s", prompt[0]);
    resp_lgth = input(response, 50);

    while (resp_lgth > 0) {

        recbuff[0] = '\0'; // zera o buffer
        append_field(recbuff, response);

        for (i=1; prompt[i] != ""; i++) {
            printf("%s", prompt[i]);
            input(response, 50);
            append_field(recbuff, response);
        }

        rec_lgth = strlen(recbuff); // calcula o tamanho do registro que está no buffer
        fprintf(fd, "%d%s", rec_lgth, recbuff); // grava o tamanho e o buffer como texto

        /* prepara nova entrada -- reiniciar o loop */
        printf("\n\n%s", prompt[0]);
        resp_lgth = input(response, 50);

    }
    fclose(fd);

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

/* função para concatenar um campo field ao buffer e 
   adicionar o delimitador de campo em seguida */
void append_field(char *buffer, char *field) {
    strcat(buffer, field);
    strcat(buffer, DELIM_STR);
}