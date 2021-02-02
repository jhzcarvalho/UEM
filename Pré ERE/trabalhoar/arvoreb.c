#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DELI "|"
#define MAXCHAVES 4
#define MINCHAVES MAXCHAVES/2

#define ENCONTRADO 1
#define NAO_ENCONTRADO 0

#define PROMOCAO 1
#define SEM_PROMOCAO 0
#define ERRO (-1)
#define tam_buf 512

//Definição de uma pagina
typedef struct {
    int contaChaves;
    int chave[MAXCHAVES];
    int offset[MAXCHAVES];
	int filho[MAXCHAVES+1];
} PAGINA;



//Inicializa uma página com configuração padrão
void inicializaPagina(PAGINA* PAG){
    int i;
    PAG->contaChaves = 0;
    for(i=0; i < MAXCHAVES; i++){
        PAG->chave[i] = 0;
        PAG->filho[i] = (-1);
    }
    PAG->filho[MAXCHAVES] = (-1);
}


//Retorna o byte offset do arquivo referente a um determinado RRN
int getOffset(int RRN){
    //(RRN * Tamanho Pagina) + Cabeçalho do Arquivo
    return ( (RRN * sizeof(PAGINA)) + sizeof(int) );
}


//Retorna o RRN do registro referente a um determinado byte offset
int getRRN(int offset){
    //(offset - cabeçalho do arquivo) / tamanho da página
    return( (offset - sizeof(int)) / sizeof(PAGINA) );
}

int novoRRN(FILE* arq){
    int offset, tamanhoPag, tamanhoCab;
    fseek(arq, 0, SEEK_END);
    offset = ftell(arq);
    tamanhoPag = sizeof(PAGINA);
    tamanhoCab = sizeof(int);
    return ( (offset-tamanhoCab)/tamanhoPag );
}

//verifica se uma pagina é folha
int ehFolha(PAGINA* PAG){
    int i;
    for(i=0; i<MAXCHAVES+1; i++){
        if(PAG->filho[i] != -1) return 0;
    }
    return 1;
}


//Insere uma chave em uma determinada página
void insereNaPagina(int CHAVE,int OFFSET, int FILHO_D, PAGINA* pag){
    int i;
    for(i=pag->contaChaves; (CHAVE < pag->chave[i-1]) && (i>0); i--){
        pag->chave[i] = pag->chave[i-1];
        pag->filho[i+1] = pag->filho[i];
        pag->offset[i] = pag->offset[i-1];
    }
    pag->contaChaves++;
    pag->chave[i] = CHAVE;
    pag->filho[i+1] = FILHO_D;
    pag->offset[i] = OFFSET;
}




void divide(FILE* arq, int CHAVE_I, int RRN_I, PAGINA* PAG, int* CHAVE_PRO, int* FILHO_D_PRO, PAGINA* NOVAPAG){
    int i, meio, chaveAux[MAXCHAVES+1], filhoAux[MAXCHAVES+2];

    for(i=0; i < MAXCHAVES; i++){
        chaveAux[i] = PAG->chave[i];
        filhoAux[i] = PAG->filho[i];
    }
    filhoAux[i] = PAG->filho[i];
    for(i=MAXCHAVES; (CHAVE_I < chaveAux[i-1]) && (i>0); i--){
        chaveAux[i] = chaveAux[i-1];
        filhoAux[i+1] = filhoAux[i];
    }
    chaveAux[i] = CHAVE_I;
    filhoAux[i+1] = RRN_I;

    *FILHO_D_PRO = novoRRN(arq);
    inicializaPagina(NOVAPAG);

    for(i=0; i < MINCHAVES; i++){
        PAG->chave[i] = chaveAux[i];
        PAG->filho[i] = filhoAux[i];
        NOVAPAG->chave[i] = chaveAux[i+1+MINCHAVES];
        NOVAPAG->filho[i] = filhoAux[i+1+MINCHAVES];
        PAG->chave[i+MINCHAVES] = 0;
        PAG->filho[i+1+MINCHAVES] = -1;
    }

    PAG->filho[MINCHAVES] = filhoAux[MINCHAVES];
    NOVAPAG->filho[MINCHAVES] = filhoAux[i+1+MINCHAVES];
    NOVAPAG->contaChaves = MAXCHAVES - MINCHAVES;
    PAG->contaChaves = MINCHAVES;
    *CHAVE_PRO = chaveAux[MINCHAVES];
}

//Cria uma página que será a nova raiz da Arvore-B, e retorna seu RRN
int novaRaiz(FILE* arq, int chave, int esquerdo, int direito){
    PAGINA* PAG = malloc(sizeof(PAGINA));
    int RRN = novoRRN(arq);
    inicializaPagina(PAG);
    PAG->chave[0] = chave;
    PAG->filho[0] = esquerdo;
    PAG->filho[1] = direito;
    PAG->contaChaves = 1;
    fseek(arq, getOffset(RRN), SEEK_SET); //Posiciona o ponteiro no registro
    fwrite(PAG, sizeof(PAGINA), 1, arq); //Grava nova pagina
    fseek(arq, 0, SEEK_SET); //Posiciona o ponteiro no inicio do arquivo
    fwrite(&RRN, sizeof(int), 1, arq); //Grava a nova raiz no cabeçalho
    return RRN;
}

//Exibe as informações de uma página
void printPagina(PAGINA* pag, int nPag, int vRaiz){
    int i;
    
    if(vRaiz == 1) printf("--------------------- PAGINA RAIZ ---------------------\n");
    
    printf("Pagina: %d\n",nPag);

    printf("Chaves: %d",pag->chave[0]);
    for(i = 1; i < MAXCHAVES; i++){
        printf(" | %d", pag->chave[i]);
    }
    printf("\n");

    printf("Offset: %d",pag->offset[0]);
    for(i = 1; i < MAXCHAVES; i++){
        printf(" | %d", pag->offset[i]);
    }
    printf("\n");

    printf("Filhos: %d",pag->filho[0]);
    for(i = 1; i < MAXCHAVES; i++){
        printf(" | %d", pag->filho[i]);
    }
    printf("\n");

    if(vRaiz == 1) printf("-------------------------------------------------------\n");
    
    /*printf("\nNumero de Chaves: %d", pag->contaChaves);
    printf("\n----------------------------------------------------------\n     ");

    for(i=0; i<MAXCHAVES; i++){
        if(pag->chave[i] == 0){
            printf("| | __ ");
        } else printf("| | %06d ", pag->chave[i]);
        if(i==MAXCHAVES-1) printf("| |");
    }

    printf("\n      /          |          |          |          \\");
    printf("\n     /           |          |          |           \\");
    printf("\n  ");

    for(i=0; i<MAXCHAVES+1; i++){
        if(i==1) printf("          ");
        if(i==2) printf("       ");
        if(i==3) printf("       ");
        if(i==4) printf("         ");
        printf("(%02d)", pag->filho[i]);
    }
    printf("\n----------------------------------------------------------\n");
    */
}




int inserir(FILE* arq, int RRN_ATUAL, int chave,int offset, int* FILHO_D_PRO, int* CHAVE_PRO){

    int i, pos=0, RRN_PRO, CHV_PRO, RETORNO;
    PAGINA* PAG = malloc(sizeof(PAGINA));
    PAGINA* NOVAPAG = malloc(sizeof(PAGINA));

    if(RRN_ATUAL == -1){ //Condição de Parada
        *CHAVE_PRO = chave;
        *FILHO_D_PRO = -1;
        return PROMOCAO;
    }
    else{
        fseek(arq, getOffset(RRN_ATUAL), SEEK_SET); //Posiciona o ponteiro no registro
        fread(PAG, sizeof(PAGINA), 1, arq); //Carrega o registro para memória
        for(i=0; i < PAG->contaChaves; i++){
            if(chave > PAG->chave[i]) pos++;
        }
    }
    if(chave == PAG->chave[pos]){
        printf("\nERRO: Nao sao permitidas chaves duplicadas.");
        return ERRO;
    }
    RETORNO = inserir(arq, PAG->filho[pos], chave, offset, &RRN_PRO, &CHV_PRO);
    if( (RETORNO == SEM_PROMOCAO) || (RETORNO == ERRO) ){
        return RETORNO;
    }
    else{
        if(PAG->contaChaves < MAXCHAVES){
            insereNaPagina(CHV_PRO, offset, RRN_PRO, PAG);
            fseek(arq, getOffset(RRN_ATUAL), SEEK_SET); //Posiciona o ponteiro no registro
            fwrite(PAG, sizeof(PAGINA), 1, arq);
            return SEM_PROMOCAO;
        }
        else{
            divide(arq, CHV_PRO, RRN_PRO, PAG, CHAVE_PRO, FILHO_D_PRO, NOVAPAG);
            fseek(arq, getOffset(RRN_ATUAL), SEEK_SET);
            fwrite(PAG, sizeof(PAGINA), 1, arq); //Escreve  PAG em RRN_ATUAL
            fseek(arq, getOffset(*FILHO_D_PRO), SEEK_SET);
            fwrite(NOVAPAG, sizeof(PAGINA), 1, arq); //Escreve  NOVAPAG em FILHO_D_PRO
            return PROMOCAO;
        }
    }
}


void listagemArvoreB(FILE* arq){

    int raiz, verificaEOF, offset, RRN, vRaiz, nPag = -1;
    PAGINA* PAG = malloc(sizeof(PAGINA));

    fseek(arq, 0, SEEK_SET);
    fread(&raiz, sizeof(int), 1, arq);
    verificaEOF = fgetc(arq);

    while(verificaEOF != EOF){ //Enquanto nao chegar fim do arquivo

        nPag++;
        vRaiz = 0;
        fseek(arq, -1, SEEK_CUR);
        offset = ftell(arq);
        RRN = getRRN(offset);
        //printf("\nRRN: %d", RRN);
        if(RRN == raiz) vRaiz = 1;
        fread(PAG, sizeof(PAGINA), 1, arq); //Le a página para memoria
        printPagina(PAG, nPag, vRaiz); //Imprime a página
        printf("\n");
        verificaEOF = fgetc(arq);
    }
}







//Cria arvore e faz a importação
void criaArvore(){
	FILE *p; //arquivo de entrada
	FILE *p2; //arquivo de saída
	short tamanho;
	char *reg_chave;
	char registro[tam_buf];
	char reg_aux[tam_buf];
	
	
	int i, numChaves, chave, offset, raiz=0, FILHO_D_PRO, CHAVE_PRO;
	
	
	if ( (p=fopen("dados.dat", "r")) ==NULL ){
		printf("\n Erro na abertura do arquivo de leitura\n");
		exit(1);
	}else printf("\nArquivo de entrada aberto com sucesso\n");
	
	if ( (p2=fopen("btree.dat", "w+")) ==NULL ){
		printf("\n Erro na abertura do arquivo de leitura\n");
		exit(1);
	}else printf("\nArquivo de saida aberto com sucesso\n");
	
	
	//implementação da arvore
	fwrite(&raiz, sizeof(int), 1, p2);
    PAGINA* PAG = malloc(sizeof(PAGINA));
    inicializaPagina(PAG);
    fwrite(PAG, sizeof(PAGINA), 1, p2);

 
    fread(&numChaves, sizeof(numChaves), 1, p);  //pega numero de chaves do arquivo de entrada
	printf("\n%d \n", numChaves);
	printf("\nNumero de chaves do arquivo de entrada: %d", numChaves);
    printf("\nIniciando importacao...");
	
	for(i=0; i<numChaves; i++){
        offset=ftell(p); //pega o offset atual
		
		fread(&tamanho, sizeof(tamanho), 1, p);
		fread(registro, sizeof(char), tamanho, p); 
	 	registro[tamanho] = '\0'; //final de string
	 	strcpy(reg_aux, registro); //Copio a String para outra pois a funcao strok embaralha o registro original
		reg_chave = strtok(registro, DELI); 
		chave= atoi(reg_chave); //converter a chave em int
	  	
		printf("\n\n");
		printf("Offset: %d \n", offset);
		printf("Registro:%s \n", reg_aux);
	  	printf("Chave %d inserida\n\n", chave);
	
    ///*
	    if( inserir(p2, raiz, chave, offset, &FILHO_D_PRO, &CHAVE_PRO) == PROMOCAO ){
            raiz = novaRaiz(p2, CHAVE_PRO, raiz, FILHO_D_PRO);
        }
        
    //*/
	}

    printf("\n\nImportacao realizada com sucesso!", numChaves);
    printf("\n%d chaves inseridas na Arvore-B.", numChaves);
	
	fclose(p);
	fclose(p2);
}




int main(int argc, char **argv) {
	FILE *p2; //arquivo de saida
	
	if (strcmp(argv[1], "-c") == 0) { //cria arvore e importa arquivos
		criaArvore();
		exit(1);
	}
	
	if (strcmp(argv[1], "-p") == 0) { //lista as paginas da arvore
		if ( (p2=fopen("btree.dat", "rb")) ==NULL ){ //
			printf("\nArquivo de dados enexistente\n");
			exit(1);
		}
		else {
			listagemArvoreB(p2);
			fclose(p2);
			exit(1);
		}

	}
	
	
	if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-c|-e|-p) nome_arquivo\n", argv[0]);
        exit(1);
    }
	


	exit(1);
}