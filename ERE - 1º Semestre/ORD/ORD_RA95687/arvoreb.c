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

//Definicao de uma pagina
typedef struct {
    int contaChaves;
    int chave[MAXCHAVES];
    int offset[MAXCHAVES];
	int filho[MAXCHAVES+1];
} PAGINA;



//Inicializa uma pagina com configuracao padrao
void inicializaPagina(PAGINA* PAG){
    int i;
    PAG->contaChaves = 0;
    for(i=0; i < MAXCHAVES; i++){
        PAG->chave[i] = 0;
        PAG->offset[i]=(-1);
		PAG->filho[i] = (-1);
    	
	}
    PAG->filho[MAXCHAVES] = (-1);
}


//Retorna o byte offset do arquivo referente a um determinado RRN
int getOffset(int RRN){
    //(RRN * Tamanho Pagina) + Cabecalho do Arquivo
    return ( (RRN * sizeof(PAGINA)) + sizeof(int) );
}


//Retorna o RRN do registro referente a um determinado byte offset
int getRRN(int offset){
    //(offset - cabecalho do arquivo) / tamanho da pagina
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

//verifica se uma pagina a folha
int ehFolha(PAGINA* PAG){
    int i;
    for(i=0; i<MAXCHAVES+1; i++){
        if(PAG->filho[i] != -1) return 0;
    }
    return 1;
}


//Insere uma chave em uma determinada pagina
void insereNaPagina(int CHAVE, int OFFSET,  int FILHO_D, PAGINA* pag){
    int i;
    for(i=pag->contaChaves; (CHAVE < pag->chave[i-1]) && (i>0); i--){
        pag->chave[i] = pag->chave[i-1];
        pag->offset[i]=pag->offset[i-1];
		pag->filho[i+1] = pag->filho[i];
    }
    pag->contaChaves++;
    pag->chave[i] = CHAVE;
	pag->offset[i] = OFFSET;
	pag->filho[i+1] = FILHO_D;
}




void divide(FILE* arq, int CHAVE_I, int OFFSET_I, int RRN_I, PAGINA* PAG, int* CHAVE_PRO, int* OFFSET_PRO, int* FILHO_D_PRO, PAGINA* NOVAPAG){
    
	int i, meio, chaveAux[MAXCHAVES+1], filhoAux[MAXCHAVES+2], offsetAux[MAXCHAVES+1];

    for(i=0; i < MAXCHAVES; i++){
        chaveAux[i] = PAG->chave[i];
        offsetAux[i]= PAG->offset[i];
		filhoAux[i] = PAG->filho[i];
    }
    filhoAux[i] = PAG->filho[i];
    for(i=MAXCHAVES; (CHAVE_I < chaveAux[i-1]) && (i>0); i--){
        chaveAux[i] = chaveAux[i-1];
        offsetAux[i]= PAG->offset[i-1];
		filhoAux[i+1] = filhoAux[i];
    }
    chaveAux[i] = CHAVE_I;
    offsetAux[i] = OFFSET_I;
	filhoAux[i+1] = RRN_I;

    *FILHO_D_PRO = novoRRN(arq);
    inicializaPagina(NOVAPAG);

    for(i=0; i < MINCHAVES; i++){
        PAG->chave[i] = chaveAux[i];
        PAG->offset[i] = offsetAux[i];
		PAG->filho[i] = filhoAux[i];
        NOVAPAG->chave[i] = chaveAux[i+1+MINCHAVES];
        NOVAPAG->offset[i] = offsetAux[i+1+MINCHAVES];
	    NOVAPAG->filho[i] = filhoAux[i+1+MINCHAVES];
        PAG->chave[i+MINCHAVES] = 0;
        PAG->offset[i+MINCHAVES] = -1;
		PAG->filho[i+1+MINCHAVES] = -1;
    }

    PAG->filho[MINCHAVES] = filhoAux[MINCHAVES];
    NOVAPAG->filho[MINCHAVES] = filhoAux[i+1+MINCHAVES];
    NOVAPAG->contaChaves = MAXCHAVES - MINCHAVES;
    PAG->contaChaves = MINCHAVES;
    *OFFSET_PRO = offsetAux[MINCHAVES];
	*CHAVE_PRO = chaveAux[MINCHAVES];
}

//Cria uma pagina que sera a nova raiz da Arvore-B, e retorna seu RRN
int novaRaiz(FILE* arq, int chave, int offset, int esquerdo, int direito){
    PAGINA* PAG = malloc(sizeof(PAGINA));
    int RRN = novoRRN(arq);
    inicializaPagina(PAG);
    PAG->chave[0] = chave;
    PAG->offset[0]=offset;
	PAG->filho[0] = esquerdo;
    PAG->filho[1] = direito;
    PAG->contaChaves = 1;
    fseek(arq, getOffset(RRN), SEEK_SET); //Posiciona o ponteiro no registro
    fwrite(PAG, sizeof(PAGINA), 1, arq); //Grava nova pagina
    fseek(arq, 0, SEEK_SET); //Posiciona o ponteiro no inicio do arquivo
    fwrite(&RRN, sizeof(int), 1, arq); //Grava a nova raiz no cabecalho
    return RRN;
}

//Exibe as informacoes de uma pagina
void printPagina(PAGINA* pag, int vRaiz, int RRN){
    int i, count = 1;
	
	if (vRaiz == 1) printf("------------------- PAGINA RAIZ -------------------\n");
	
	printf("Pagina: %d\n", RRN);
	
	printf("Chaves: %d ", pag->chave[0]);
	for (i = 1; i < MAXCHAVES; i++){
		if (pag->chave[i] != 0){
			printf ("| %d ", pag->chave[i]);
		}
	}
	printf("\n");

	printf("Offset: %d ", pag->offset[0]);
	for (i = 1; i < MAXCHAVES; i++){
		if (pag->offset[i] != -1){
			printf ("| %d ", pag->offset[i]);
			count++;
		}
	}
	printf("\n");

	printf("Filhos: %d ", pag->filho[0]);
	for (i = 1; i < count+1; i++){
		printf ("| %d ", pag->filho[i]);
	}
	printf("\n");

	if (vRaiz == 1) printf("---------------------------------------------------\n");

}



int inserir(FILE* arq, int RRN_ATUAL, int chave, int offset, int* FILHO_D_PRO, int* CHAVE_PRO, int* OFFSET_PRO){

    int i, pos=0, RRN_PRO, CHV_PRO, OFFST_PRO, RETORNO;
    PAGINA* PAG = malloc(sizeof(PAGINA));
    PAGINA* NOVAPAG = malloc(sizeof(PAGINA));

    if(RRN_ATUAL == -1){ //Condicao de Parada
        *CHAVE_PRO = chave;
        *OFFSET_PRO = offset;
		*FILHO_D_PRO = -1;
        return PROMOCAO;
    }
    else{
        fseek(arq, getOffset(RRN_ATUAL), SEEK_SET); //Posiciona o ponteiro no registro
        fread(PAG, sizeof(PAGINA), 1, arq); //Carrega o registro para memoria
        for(i=0; i < PAG->contaChaves; i++){
            if(chave > PAG->chave[i]) pos++;
        }
    }
    if(chave == PAG->chave[pos]){
        printf("\nERRO: Nao sao permitidas chaves duplicadas.");
        return ERRO;
    }
    RETORNO = inserir(arq, PAG->filho[pos], chave, offset, &RRN_PRO, &CHV_PRO, &OFFST_PRO);
    if( (RETORNO == SEM_PROMOCAO) || (RETORNO == ERRO) ){
        return RETORNO;
    }
    else{
        if(PAG->contaChaves < MAXCHAVES){
            insereNaPagina(CHV_PRO, OFFST_PRO, RRN_PRO, PAG);
            fseek(arq, getOffset(RRN_ATUAL), SEEK_SET); //Posiciona o ponteiro no registro
            fwrite(PAG, sizeof(PAGINA), 1, arq);
            return SEM_PROMOCAO;
        }
        else{
            divide(arq, CHV_PRO, OFFST_PRO, RRN_PRO, PAG, CHAVE_PRO, OFFSET_PRO, FILHO_D_PRO, NOVAPAG);
            fseek(arq, getOffset(RRN_ATUAL), SEEK_SET);
            fwrite(PAG, sizeof(PAGINA), 1, arq); //Escreve  PAG em RRN_ATUAL
            fseek(arq, getOffset(*FILHO_D_PRO), SEEK_SET);
            fwrite(NOVAPAG, sizeof(PAGINA), 1, arq); //Escreve  NOVAPAG em FILHO_D_PRO
            return PROMOCAO;
        }
    }
}


void listagemArvoreB(FILE* arq){

    int raiz, verificaEOF, offset, RRN, vRaiz;
    PAGINA* PAG = malloc(sizeof(PAGINA));

    fseek(arq, 0, SEEK_SET);
    fread(&raiz, sizeof(int), 1, arq);
    verificaEOF = fgetc(arq);

    while(verificaEOF != EOF){ //Enquanto nao chegar fim do arquivo

		vRaiz =0;
        fseek(arq, -1, SEEK_CUR);
        offset = ftell(arq);
        RRN = getRRN(offset);
        if(RRN == raiz) vRaiz = 1;
        fread(PAG, sizeof(PAGINA), 1, arq); //Le a pagina para memoria
        printPagina(PAG, vRaiz, RRN); //Imprime a pagina
        printf("\n");
        verificaEOF = fgetc(arq);
    }
}







//Cria arvore e faz a importacao
void criaArvore(){
	FILE *p; //arquivo de entrada
	FILE *p2; //arquivo de saida
	short tamanho;
	char *reg_chave;
	char registro[tam_buf];
	char reg_aux[tam_buf];
	
	
	int i, numChaves, chave, offset, raiz=0, FILHO_D_PRO, CHAVE_PRO, OFFSET_PRO;
	
	
	if ( (p=fopen("dados.dat", "rb")) ==NULL ){
		printf("\n Erro na abertura do arquivo de leitura\n");
		exit(1);
	}else printf("\nArquivo de entrada aberto com sucesso\n");
	
	if ( (p2=fopen("btree.dat", "wb+")) ==NULL ){
		printf("\n Erro na abertura do arquivo de leitura\n");
		exit(1);
	}else printf("\nArquivo de saida aberto com sucesso\n");
	
	
	//implementacao da arvore
	fwrite(&raiz, sizeof(int), 1, p2);
    PAGINA* PAG = malloc(sizeof(PAGINA));
    inicializaPagina(PAG);
    fwrite(PAG, sizeof(PAGINA), 1, p2);

 
    fread(&numChaves, sizeof(numChaves), 1, p);  //pega numero de chaves do arquivo de entrava
	printf("\nNumero de chaves do arquivo de entrada: %d", numChaves);
    printf("\nIniciando importacao...\n");
	
	for(i=0; i<numChaves; i++){
        offset=ftell(p); //pega o offset atual
		
		fread(&tamanho, sizeof(tamanho), 1, p);
		fread(registro, sizeof(char), tamanho, p); 
	 	registro[tamanho] = '\0'; //final de string
	 	strcpy(reg_aux, registro); //Copio a String para outra pois a funcao strok embaralha o registro original
		reg_chave = strtok(registro, DELI); 
		chave= atoi(reg_chave); //converter a chave em int
	  	

	    if( inserir(p2, raiz, chave, offset, &FILHO_D_PRO, &CHAVE_PRO, &OFFSET_PRO) == PROMOCAO ){
            
			raiz = novaRaiz(p2, CHAVE_PRO, OFFSET_PRO, raiz, FILHO_D_PRO);
        }
        
	}

    printf("\n\nImportacao realizada com sucesso!", numChaves);
    printf("\n%d chaves inseridas na Arvore-B. \n\n", numChaves);
	
	fclose(p);
	fclose(p2);
}

int buscar( FILE *p1, FILE *p2, int RRN, int chave, int* RRN_ENCONTRADO, int* POS_ENCONTRADA){
	
	int i, pos=0;
    PAGINA* PAG = malloc(sizeof(PAGINA));

    if(RRN == -1){ //Condicao de Parada
       // printf("\n Erro: registro nao encontrado! \n");	
		return NAO_ENCONTRADO;
    }
    else{
        fseek(p2, getOffset(RRN), SEEK_SET); //Posiciona o ponteiro no registro
        fread(PAG, sizeof(PAGINA), 1, p2); //Carrega o registro para memoria
        for(i=0; i < PAG->contaChaves; i++){
            if(chave > PAG->chave[i]) pos++;
        }
        if(chave == PAG->chave[pos]){
            short tamanho;
			char *reg_chave;
			char registro[tam_buf];
			char reg_aux[tam_buf];
			int offset = PAG->offset[pos];
		    
			
			*RRN_ENCONTRADO = RRN;
            *POS_ENCONTRADA = pos;
            
			fseek(p1, offset, SEEK_SET);
		    fread(&tamanho, sizeof(tamanho), 1, p1);
			
			
			fread(registro, sizeof(char), tamanho, p1); 
	 		registro[tamanho] = '\0'; //final de string
	 		strcpy(reg_aux, registro); //Copio a String para outra pois a funcao strok embaralha o registro original
			reg_chave = strtok(registro, DELI); 
			chave= atoi(reg_chave); //converter a chave em int
	  	
			printf(" \n %s (%d bytes) ", reg_aux, tamanho);
			
			return ENCONTRADO;
        }
        else{
			return (buscar(p1, p2, PAG->filho[pos], chave, RRN_ENCONTRADO, POS_ENCONTRADA));
        }
    }
}

short ler_reg(char buff[], FILE *reg){ //funcao que le os registro do arquivo, troca delimitador de ; para |, retorna o trabalho do buffer
	short k=0;
	char c;
	c=fgetc(reg);
	while (c!='\n' && c!=EOF){
	    if(c==';'){
			c='|';
			
		}
		buff[k] = c;
	 	k++;
		c=fgetc(reg);
	}
 	
	buff[k]='\0';
 	
	if (k>0){
			 if (buff[k-1]!= '|'){
			 
			 	buff[k]='|';
			 	k++;
				buff[k]='\0';
		     }else {
			    buff[k]='\0';
			 }
			 
		     
			 //printf("\n\nString lida de arquivo para o buffer: %s\n", buff);	
	}
			
	 return k; 
}



void insereReg(FILE* p, FILE* p2, char buffer[tam_buf], int raiz){
	
	char buffer_aux[tam_buf];
    
	strcpy(buffer_aux, buffer); //copia pra buffer_aux pois strtok zoa funcao
	
	char *token=strtok(buffer_aux, DELI); //quebra o primeiro registro do buffer
	
	int chave=atoi(token); //transforma em int a chave
	
	short tam_reg;
	
	int total_reg; 
	
	tam_reg=strlen(buffer); //pega o tamanho da String
	
	int offset;
	
	int rrnEncontrado, posEncontrada, FILHO_D_PRO, CHAVE_PRO, OFFSET_PRO;
	printf("\n Insercao do registro de chave %d (%d bytes)", chave, tam_reg);
	
	if ((buscar(p, p2, raiz, chave, &rrnEncontrado, &posEncontrada)) ==ENCONTRADO){
		printf("\n Erro: Ja existe um registro com essa chave (%d)\n", chave);	
		fclose(p);
		fclose(p2);
		return;
	}

	fseek(p, 0, SEEK_SET);
	fread(&total_reg, sizeof(total_reg), 1, p);
	total_reg++;
	
	fseek(p, 0, SEEK_SET);
	fwrite(&total_reg, sizeof(total_reg), 1, p);
	
	fseek(p, 0, SEEK_END);
	offset=ftell(p);
	
	fseek(p, 0, SEEK_END);
	fwrite(&tam_reg, sizeof(tam_reg), 1, p);
	
	fseek(p, 0, SEEK_END);
	fwrite(buffer, sizeof(char), tam_reg, p);
	

	
	
	if( inserir(p2, raiz, chave, offset, &FILHO_D_PRO, &CHAVE_PRO, &OFFSET_PRO) == PROMOCAO ){
            raiz = novaRaiz(p2, CHAVE_PRO, OFFSET_PRO, raiz, FILHO_D_PRO);
    }
	fclose(p);
	fclose(p2);

}




int main(int argc, char **argv) {
	FILE *p; //arquivo de entrada
	FILE *p2; //arquivo de saida
	FILE *p3; //arquivo de cmd
	
	char buffer[tam_buf];
	short tam_reg;
	int led=-1;
	int offset_reg=0; //contador de offsets
	
	
	
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
	
	if (strcmp(argv[1], "-e") == 0) { //lista as paginas da arvore
		
		
		
		if (argc < 3) {
	        fprintf(stderr, "Numero incorreto de argumentos!\n");
	        fprintf(stderr, "Modo de uso:\n");
	        fprintf(stderr, "$ %s (-c|-e|-p) nome_arquivo\n", argv[0]);
	        exit(1);
    	}
		
		
		
		if ( (p3=fopen(argv[2], "r")) ==NULL ){
			printf("\n Erro na abertura do arquivo de leitura de comandos\n");
			exit(1);
		}else printf("\n Arquivo de comandos aberto com sucesso\n");
		
		tam_reg=ler_reg(buffer, p3);
				while (tam_reg > 0){
					char buffer_aux[tam_buf];
					strcpy(buffer_aux, buffer);
					
					
					char *token=strtok(buffer, "");
					int op_menu=atoi(token);
					
					int tam_buffer_aux=(int)strlen(buffer_aux);
					char buffer_menu[tam_buf];
					int aux=0;
					int k=2;
					for(k;k<tam_buffer_aux;k++){
					   buffer_menu[aux]=buffer_aux[k];
					   aux++;
					}
					int chave_menu;
					
					if(op_menu!=2){
						buffer_menu[aux-1]='\0';
						chave_menu=atoi(buffer_menu);
					}
					
					if(op_menu==2){
						buffer_menu[aux]='\0';
					}
					
					if(op_menu==1){
						printf("\n");
						 int rrnEncontrado, posEncontrada, raiz;
						
						if ( (p=fopen("dados.dat", "rb")) ==NULL ){
							printf("\n Erro na abertura do arquivo de leitura\n");
							exit(1);
						}
						  
						if ( (p2=fopen("btree.dat", "rb")) ==NULL ){ 
							printf("\nArquivo de dados enexistente\n");
							exit(1);
						}
						
						fread(&raiz, sizeof(int), 1, p2);
						printf("\n Busca pelo registro de chave:  %d ", chave_menu );
						
						if ((buscar(p, p2, raiz, chave_menu, &rrnEncontrado, &posEncontrada)) ==NAO_ENCONTRADO){
						   		printf("\n Erro: registro nao encontrado! \n");	
						}	
						  
						fclose(p);
						fclose(p2);
				
					}
					
					if(op_menu==2){
					 	printf("\n");
						int rrnEncontrado, posEncontrada, raiz;
					 	
						if ( (p=fopen("dados.dat", "rb+")) ==NULL ){
						  	printf("\n Erro na abertura do arquivo de leitura\n");
						  	exit(1);
						}
						  
						if ( (p2=fopen("btree.dat", "rb+")) ==NULL ){ 
							printf("\nArquivo de dados enexistente\n");
							exit(1);
						}
						   
						fread(&raiz, sizeof(int), 1, p2);
						insereReg(p, p2, buffer_menu, raiz);
						   
						   	
						fclose(p);
						fclose(p2);	
					}
					
					buffer[0]='\0';
					tam_reg=ler_reg(buffer, p3);
				
				}
	
		
		fclose(p3);
	} else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }
	exit(1);
}
