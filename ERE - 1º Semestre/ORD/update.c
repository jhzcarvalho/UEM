/* programa que abre ou cria um arquivo de registros de tamanho fixo.
   Os registros podem ser adicionados ou modificados.
   Registros a serem modificados são buscados por RRN */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DELIM_STR "|"
#define REC_LGTH 64

char *prompt[] = {
    "    Sobrenome: ",
    "Primeiro nome: ",
    "     Endereco: ",
    "       Cidade: ",
    "       Estado: ",
    "          CEP: ",
    "" // string vazia para encerrar o loop
};

// GLOBAIS
struct {
    int rec_count;
} header; // cabeçalho do arquivo - 4 bytes

FILE *fd;

// INTERFACES
int menu();
void ask_info(char *);
int ask_rrn();
void read_and_show();
int change();
char ucase(char);
int input(char *str, int size);
int input_int();
char input_char();
void append_field(char *buffer, char *field);


// IMPLEMENTAÇÕES
int main() {

    int i, menu_choice, rrn;
    int byte_offset;
    char filename[21];
    char recbuff[REC_LGTH+1];

    printf("Digite o nome do arquivo: ");
    input(filename, 20);

    if ((fd = fopen(filename, "r+")) == NULL) {
        fd = fopen(filename, "w+");
        header.rec_count = 0;
        fwrite(&header, sizeof(header), 1, fd);
    }
    else
        fread(&header, sizeof(header), 1, fd);

    while ((menu_choice = menu()) < 3) {

        switch(menu_choice) {
            case 1: // inserção
                printf("Digite os dados para o novo registro\n\n");
                ask_info(recbuff);
                byte_offset = header.rec_count * REC_LGTH + sizeof(header);
                fseek(fd, (long) byte_offset, SEEK_SET);
                fwrite(recbuff, REC_LGTH, 1, fd);
                header.rec_count++;
                break;

            case 2: // busca e alteração
                rrn = ask_rrn();

                if (rrn >= header.rec_count){
                    printf("O RRN eh muito grande... RRN max = %d", header.rec_count-1);
                    printf("... retornando ao menu ...");
                    break;
                }

                byte_offset = rrn * REC_LGTH + sizeof(header);
                fseek(fd, (long) byte_offset, SEEK_SET);

                read_and_show();

                if (change()){
                    printf("\n\nDigite os novos dados do registro: \n\n");
                    ask_info(recbuff);
                    fseek(fd, (long) byte_offset, SEEK_SET);
                    fwrite(recbuff, REC_LGTH, 1, fd);
                }
                break;
        }
    }

    rewind(fd);
    fwrite(&header, sizeof(header), 1, fd);

    fclose(fd);

    return EXIT_SUCCESS;
}

/* função local para perguntar ao usuario pelal próx. operação
   retorna um valor numérico */
int menu() {

    int choice;
    char str[5];
    
    printf("\n\nPROGRAMA PARA CRIACAO E ALTERACAO DE ARQUIVO\n");
    printf("\n\nSuas opcoes sao:\n\n");
    printf("\t1. Adicionar um registro ao final do arquivo\n");
    printf("\t2. Recuperar um registro por RRN para alteracoes \n");
    printf("\t3. Terminar o programa\n\n");
    printf("Digite o numero da sua escolha: ");
    choice = input_int();
    return choice;

} // end menu

/* função para a entrada de um nome e um endereço,
   escrevendo-os no buffer passado por parâmetro */
void ask_info(char recbuff[]) {

    int field_count, i;
    char response[51];

    // clear record buffer
    recbuff[0] = '\0';

    // get fields
    for (i = 0; prompt[i] != ""; i++) {
        printf("%s", prompt[i]);
        input(response, 50);
        append_field(recbuff, response);
    }
}

/* função para a entrada do RRN do registro a ser alterado */
int ask_rrn() {
    int rrn;

    printf("\n\nDigite o RRN do registro: ");
    rrn = input_int();
    return rrn;
}

/* função que lê e mostra um registro, sem seek.
   a leitura começa na posição atual do arquivo */
void read_and_show() {

    char recbuff[REC_LGTH+1];
    char *field;

    fread(recbuff, REC_LGTH, 1, fd);

    printf("\n\n\n\nConteudo do registro\n");
    field = strtok(recbuff, "|");
    while (field != NULL) {
        printf("\t%s\n",field);
        field = strtok(NULL, "|");
    }
}

/* função para perguntar ao usuário se ele quer modificar o registro ou nao
   Retorna 1 para YES e 0 para NO */
int change() {

    char response;

    printf("\n\nVoce quer modificar este registro?\n");
    printf("    Responda S ou N, seguido de <ENTER>: ");
    response = ucase(input_char());
    return ((response == 'S') ? 1 : 0);
}

/* Converte o char si para UPPERCASE e retorna em so */
char ucase(char si){
    char so = (si >= 'a' && si <= 'z') ? si & 0x5f : si;
    return so;
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

/* função que lê e retorna um número inteiro do stdin */
int input_int() {
    char str[25];
    if (input(str, 24) > 0)
        return atoi(str);
    return 0;
}

/* função que lê e retorna um char do stdin */
char input_char() {
    char str[2];
    input(str, 1);
    return str[0];
}

/* função para concatenar um campo field ao buffer e 
   adicionar o delimitador de campo em seguida */
void append_field(char *buffer, char *field) {
    strcat(buffer, field);
    strcat(buffer, DELIM_STR);
}