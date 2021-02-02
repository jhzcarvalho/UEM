#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REC_SIZE 512

int input(char *str, int size);
short read_rec(char *recbuff, FILE *fd);

int main() {

    FILE *fd;
    int rec_count, fld_count;
    int rec_lgth;
    char filename[21];
    char recbuff[MAX_REC_SIZE + 1];
    char *field;

    printf("Entre com o nome do arquivo a ser aberto: ");
    input(filename, 20);

    if ((fd = fopen(filename, "rb")) == NULL) {
       printf("Erro na abertura do arquivo --- programa abortado.\n");
       exit(1);
    }

    rec_count = 0;

    while ((rec_lgth = read_rec(recbuff, fd)) > 0) {

        printf("Registro %d\n", ++rec_count);
        fld_count = 0;

        field = strtok(recbuff,"|");
        while (field != NULL) {
            printf("> Campo %d: %s\n", ++fld_count, field);
            field = strtok(NULL, "|");
        }

    }

    fclose(fd);

    return EXIT_SUCCESS;
}

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

    if (fscanf(fd, "%d", &rec_lgth) == EOF) // get record length
        return 0;

    rec_lgth = fread(recbuff, sizeof(char), rec_lgth, fd); // read record
    recbuff[rec_lgth] = '\0';

    return rec_lgth;
}