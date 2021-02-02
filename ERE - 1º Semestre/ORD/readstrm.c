/* lê uma sequencia de campos delimitados por "|" */

#include <stdio.h>
#include <stdlib.h>

#define DELIM_CHR '|'

int readfield(FILE *fd, char *str, int size);
int input(char *str, int size);

int main() {

    FILE *fd;
    char field[31];
    char filename[21];
    int field_count;

    printf("Entre com o nome do arquivo a ser lido: ");
    input(filename, 20);

    if ((fd = fopen(filename, "r")) == NULL) {
       printf("Erro na abertura do arquivo --- programa abortado.\n");
       exit(1);
    }

    /* loop do programa principal -- chama a função 'readfield'
       enquanto a função tiver sucesso (retorno > 0) */
    field_count = 0;
    while (readfield(fd, field, 30) > 0)
        printf("> campo #%d: %s\n", ++field_count, field);

    fclose(fd);

    return EXIT_SUCCESS;
}

/* função que lê um campo na string field
   e retorna o tamanho da string lida
*/
int readfield(FILE *fd, char *str, int size) {

    char c;
    int i = 0;

    c = fgetc(fd);
    while ( c != EOF && c != DELIM_CHR ) {
        if (size > 0) {
            str[i] = c;
            i++;
            size--;
        }
        c = fgetc(fd);
    }

    str[i] = '\0'; // anexa 'null' ao final da string
    return i;
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
