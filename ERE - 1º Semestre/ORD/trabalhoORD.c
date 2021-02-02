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


int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-i") == 0) {
        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);

        FILE *inputFile;
        if ((inputFile = fopen(argv[2], "r")) == NULL) {
           printf("Erro na abertura do arquivo --- programa abortado.\n");
           exit(1);
        }



        /*
        -ABRIR ARQUIVO argv[2]
        -IMPORTAR TODOS OS CAMPOS E ARMAZERNAR
        EM UM REGISTRO USANDO | COMO SEPARADOR
        -MOSTRAR MENSAGEM DE IMPORTACAO CONCLUIDA
                                                  */
    } else if (strcmp(argv[1], "-e") == 0) {
        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);
        /*
        -ABRIR ARQUIVO argv[2]
        -IMPORTAR TODOS OS CAMPOS E ARMAZERNAR
        EM UM REGISTRO USANDO | COMO SEPARADOR
        -MOSTRAR MENSAGEM DE IMPORTACAO CONCLUIDA
                                                  */
    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}
