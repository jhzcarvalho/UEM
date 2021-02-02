#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void criaArvore(){
	FILE *p; //arquivo de entrada
	FILE *p2; //arquivo de saída
	
	printf("Entrou aqui");
	
	if ( (p=fopen("dados.dat", "r")) ==NULL ){
		printf("\n Erro na abertura do arquivo de leitura\n");
		exit(1);
	}else printf("\nArquivo de entrada aberto com sucesso\n");
	
	if ( (p2=fopen("btree.dat", "wb")) ==NULL ){
		printf("\n Erro na abertura do arquivo de leitura\n");
		exit(1);
	}else printf("\nArquivo de saida aberto com sucesso\n");
	
	
	//implementação da arvore
	
	
	
	
	
	
	
	
	fclose(p);
	fclose(p2);
}


int main(int argc, char **argv) {
	
	if (strcmp(argv[1], "-c") == 0) {
		criaArvore();
		exit(1);
	}
	if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        exit(1);
    }
	


	exit(1);
}