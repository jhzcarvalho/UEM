/* faz busca sequencial por uma chave primária em um arquivo de registros
   no formato gravado por writrec-fwrite.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  TRUE     1
#define  FALSE    0
#define MAX_REC_SIZE 512

/* função que lê uma linha do stdin para a string str
*/
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


/* função que lê um registro do arquivo fd e o armazena em recbuff
*/
short read_rec(char *recbuff, FILE *fd) {
    int rec_lgth;

    if (fread(&rec_lgth, sizeof(rec_lgth), 1, fd) == 0) // get record length
        return 0;

    rec_lgth = fread(recbuff, sizeof(char), rec_lgth, fd); // read record
    recbuff[rec_lgth] = '\0';

    return rec_lgth;
}

int main() {

    FILE *fd;
    int matched;
    int rec_lgth;
    char filename[21];
    char search_key[30];
    char recbuff[MAX_REC_SIZE + 1];
    char *field, *last;

    printf("Entre com o nome do arquivo a ser aberto: ");
    input(filename, 20);

    if ((fd = fopen(filename, "rb")) == NULL) {
        printf("Erro na abertura do arquivo --- programa abortado.\n");
        exit(1);
    }

    printf("\n\nDigite o SOBRENOME a ser buscado: "); // get search key
    input(search_key, 30);

    matched = FALSE;
    while (!matched && (rec_lgth = read_rec(recbuff, fd)) > 0) {
        last = strtok(recbuff, "|");
        if (strcmp (last, search_key) == 0)
            matched = TRUE;
    }

    if (matched) {
        printf("\n\nRegistro encontrado:\n\n");

        int fld_count = 0;
        printf("\tCampo %d: %s\n", ++fld_count, last);

        while ((field = strtok(NULL, "|")) != NULL) {
            printf("\tCampo %d: %s\n", ++fld_count, field);
        }
    }
    else printf("\n\nRegistro nao encontrado.\n");

    fclose(fd);

    return EXIT_SUCCESS;
}
