#include<stdio.h>
#include<string.h>

void criaArvore(){
    
}

void buscaInsere(){

}

int main(int argc, char **argv){
    
    if (strcmp(argv[1], "-c") == 0) {
        printf("\n Cria Arvore \n\n");
    }else if (strcmp(argv[1], "-e") == 0) {
        printf("\n Busca e insere \n\n");
    }else{
        printf("\n\n Insira um comando valido, terminando programa\n\n");
    }
}