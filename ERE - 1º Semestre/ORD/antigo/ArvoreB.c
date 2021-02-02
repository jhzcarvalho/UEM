#include <stdio.h>
#include <stdlib.h>

#define MAXCHAVES 4
#define MINCHAVES MAXCHAVES/2

#define ENCONTRADO 1
#define NAO_ENCONTRADO 0

#define PROMOCAO 1
#define SEM_PROMOCAO 0
#define ERRO (-1)


//Definição de uma pagina
typedef struct {
    int contaChaves;
    int chave[MAXCHAVES];
    int filho[MAXCHAVES+1];
} PAGINA;


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


/*
//Exibe as informações de uma página
void printPagina(PAGINA* pag){
    int i;
    printf("\n----------------------");
    printf("\nNumero de Chaves: %d\n", pag->contaChaves);
    for(i=0; i<pag->contaChaves; i++){
        printf("\nChave[%d]: %d", i, pag->chave[i]);
    }
    printf("\n");
    for(i=0; i<MAXCHAVES+1; i++){
        printf("\nFilho[%d]: %d", i, pag->filho[i]);
    }
    printf("\n----------------------\n");
}
*/


//Exibe as informações de uma página
void printPagina(PAGINA* pag){
    int i;
    printf("\nNumero de Chaves: %d", pag->contaChaves);
    printf("\n----------------------------------------------------------\n     ");

    for(i=0; i<MAXCHAVES; i++){
        if(pag->chave[i] == 0){
            printf("| | ______ ");
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
void insereNaPagina(int CHAVE, int FILHO_D, PAGINA* pag){
    int i;
    for(i=pag->contaChaves; (CHAVE < pag->chave[i-1]) && (i>0); i--){
        pag->chave[i] = pag->chave[i-1];
        pag->filho[i+1] = pag->filho[i];
    }
    pag->contaChaves++;
    pag->chave[i] = CHAVE;
    pag->filho[i+1] = FILHO_D;
}


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


//Retorna o RRN para um novo registro no arquivo
int novoRRN(FILE* arq){
    int offset, tamanhoPag, tamanhoCab;
    fseek(arq, 0, SEEK_END);
    offset = ftell(arq);
    tamanhoPag = sizeof(PAGINA);
    tamanhoCab = sizeof(int);
    return ( (offset-tamanhoCab)/tamanhoPag );
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


//Busca uma Chave na Arvore-B, retorna 1 caso encontrado e 0 caso contrário
int buscar(FILE* arq, int RRN, int chave, int* RRN_ENCONTRADO, int* POS_ENCONTRADA){

    int i, pos=0;
    PAGINA* PAG = malloc(sizeof(PAGINA));

    if(RRN == -1){ //Condição de Parada
        printf("\nNenhum resultado encontrado para a chave %d.", chave);
        return NAO_ENCONTRADO;
    }
    else{
        fseek(arq, getOffset(RRN), SEEK_SET); //Posiciona o ponteiro no registro
        fread(PAG, sizeof(PAGINA), 1, arq); //Carrega o registro para memória
        for(i=0; i < PAG->contaChaves; i++){
            if(chave > PAG->chave[i]) pos++;
        }
        if(chave == PAG->chave[pos]){
            *RRN_ENCONTRADO = RRN;
            *POS_ENCONTRADA = pos;
            printf("\nRRN Encontrado: %d", RRN);
            printf("\nPOS Encontrada: %d", pos);
            printPagina(PAG);
            return ENCONTRADO;
        }
        else{
            return (buscar(arq, PAG->filho[pos], chave, RRN_ENCONTRADO, POS_ENCONTRADA));
        }
    }
}


//Insere uma chave na Arvore-B
int inserir(FILE* arq, int RRN_ATUAL, int chave, int* FILHO_D_PRO, int* CHAVE_PRO){

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
    RETORNO = inserir(arq, PAG->filho[pos], chave, &RRN_PRO, &CHV_PRO);
    if( (RETORNO == SEM_PROMOCAO) || (RETORNO == ERRO) ){
        return RETORNO;
    }
    else{
        if(PAG->contaChaves < MAXCHAVES){
            insereNaPagina(CHV_PRO, RRN_PRO, PAG);
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


//Forma uma Arvore-B para o arquivo saida a partir de um arquivo de chaves entrada
void importar(FILE* entrada, FILE* saida){

    int i, numChaves, chave, raiz=0, FILHO_D_PRO, CHAVE_PRO;

    fwrite(&raiz, sizeof(int), 1, saida);
    PAGINA* PAG = malloc(sizeof(PAGINA));
    inicializaPagina(PAG);
    fwrite(PAG, sizeof(PAGINA), 1, saida);

    fscanf(entrada, "%d", &numChaves);
    printf("\nNumero de chaves do arquivo de entrada: %d", numChaves);
    printf("\nIniciando importacao...");

    for(i=0; i<numChaves; i++){
        fscanf(entrada, "%d", &chave);
        if( inserir(saida, raiz, chave, &FILHO_D_PRO, &CHAVE_PRO) == PROMOCAO ){
            raiz = novaRaiz(saida, CHAVE_PRO, raiz, FILHO_D_PRO);
        }
        printf("\nChave %d inserida", chave);
    }

    printf("\n\nImportacao realizada com sucesso!", numChaves);
    printf("\n%d chaves inseridas na Arvore-B.", numChaves);

}


//Imprime todas as páginas da Arvore-B informando a raiz da árvore, e o RRN de cada página
void listagemArvoreB(FILE* arq){

    int raiz, verificaEOF, offset, RRN;
    PAGINA* PAG = malloc(sizeof(PAGINA));

    fseek(arq, 0, SEEK_SET);
    fread(&raiz, sizeof(int), 1, arq);
    verificaEOF = fgetc(arq);

    while(verificaEOF != EOF){ //Enquanto nao chegar fim do arquivo

        fseek(arq, -1, SEEK_CUR);
        offset = ftell(arq);
        RRN = getRRN(offset);
        printf("\nRRN: %d", RRN);
        if(RRN == raiz) printf(" (Raiz da Arvore)");
        fread(PAG, sizeof(PAGINA), 1, arq); //Le a página para memoria
        printPagina(PAG); //Imprime a página
        printf("\n");
        verificaEOF = fgetc(arq);
    }
}


//-------------------------- INTERFACES DO USUÁRIO --------------------------//


int solicitaImportar(char* nomeEntrada, char* nomeSaida){

    fflush(stdin);
    printf("\n+----------------------------------------------------+");
    printf("\n|                IMPORTAR ARVORE-B                   |");
    printf("\n+----------------------------------------------------+");
    printf("\nDigite o nome do arquivo com chaves que sera importado: ");
    scanf("%s", nomeEntrada);
    printf("\nDigite o nome do arquivo em que sera formada a arvore: ");
    scanf("%s", nomeSaida);

    FILE* entrada = fopen(nomeEntrada, "r");
    if(entrada==NULL){
        printf("\nERRO: Arquivo de Origem \"%s\" nao encontrado.", nomeEntrada);
        return 0;
    }
    FILE* saida = fopen(nomeSaida, "wb+");
    importar(entrada, saida);
    fclose(entrada);
    fclose(saida);
    return 1;
}


int solictaInserir(char* nomeSaida){

    FILE* saida = fopen(nomeSaida, "rb+");
    if(saida==NULL){
        printf("\nERRO: Arquivo \"%s\" nao encontrado.", nomeSaida);
        return 0;
    }

    int chave, raiz, FILHO_D_PRO, CHAVE_PRO, retorno;

    fread(&raiz, sizeof(int), 1, saida);
    printf("\nTeste raiz: %d", raiz);

    fflush(stdin);
    printf("\n+----------------------------------------------------+");
    printf("\n|            INSERIR CHAVE NA ARVORE-B               |");
    printf("\n+----------------------------------------------------+");

    printf("\nDigite a chave a ser inserida: ");
    scanf("%d", &chave);

    retorno = inserir(saida, raiz, chave, &FILHO_D_PRO, &CHAVE_PRO);
    if(retorno == PROMOCAO){
        raiz = novaRaiz(saida, CHAVE_PRO, raiz, FILHO_D_PRO);
    }
    if(retorno != ERRO) printf("\nChave %d inserida com sucesso!", chave);
    fclose(saida);
    return 1;
}


int solicitaBuscar(char* nomeSaida){

    FILE* saida = fopen(nomeSaida, "rb");
    if(saida==NULL){
        printf("\nERRO: Arquivo \"%s\" nao encontrado.", nomeSaida);
        return 0;
    }

    int rrnEncontrado, posEncontrada, raiz, chave;

    fflush(stdin);
    printf("\n+----------------------------------------------------+");
    printf("\n|             BUSCAR CHAVE NA ARVORE-B               |");
    printf("\n+----------------------------------------------------+");

    printf("\nDigite a chave a ser buscada: ");
    scanf("%d", &chave);

    fread(&raiz, sizeof(int), 1, saida);
    buscar(saida, raiz, chave, &rrnEncontrado, &posEncontrada);
    fclose(saida);
    return 1;
}


int solicitaImprimir(char* nomeSaida){

    FILE* saida = fopen(nomeSaida, "rb");
    if(saida==NULL){
        printf("\nERRO: Arquivo \"%s\" nao encontrado.", nomeSaida);
        return 0;
    }

    printf("\n+----------------------------------------------------+");
    printf("\n|               LISTAGEM DA ARVORE-B                 |");
    printf("\n+----------------------------------------------------+\n");

    listagemArvoreB(saida);
    fclose(saida);
    return 1;
}


void exibirMenu(){
    printf("\n\n  1 - Criar Arvore-B (importar)");
    printf("\n  2 - Editar arquivo Arvore-B existente\n");
    printf("\n  3 - Inserir chave");
    printf("\n  4 - Buscar Chave");
    printf("\n  5 - Imprimir Arvore-B\n");
    printf("\n  0 - Sair\n");
}


void exibirMenuInicial(){
    printf("\n\n  1 - Criar Arvore-B (importar)");
    printf("\n  2 - Editar arquivo Arvore-B existente\n");
    printf("\n  0 - Sair\n");
}


int main(){

    int opcao=1, arquivoAberto=0;
    char nomeEntrada[30], nomeSaida[30];
    strcpy(nomeSaida, "Nenhum arquivo aberto");

    printf("+----------------------------------------------------+");
    printf("\n|            Gerenciamento de Arvore-B               |");
    printf("\n|        Por: William Rodrigues e Joao Vitor         |");
    printf("\n+----------------------------------------------------+");

    while(opcao!=0){
        printf("\n\n------------------------------------------------------");
        printf("\nInfo: Arquivo sendo editado: %s", nomeSaida);
        if(arquivoAberto) exibirMenu();
        else exibirMenuInicial();
        printf("\n------------------------------------------------------");
        printf("\nOpcao: ");
        scanf("%d", &opcao);
        fflush(stdin);

        switch(opcao){

        case 0: //Sair
            printf("\n\nObrigado por usar o sistema!\n\n");
            break;

        case 1: //Importar
            if(solicitaImportar(nomeEntrada, nomeSaida)) arquivoAberto = 1;
            else{
                arquivoAberto = 0;
                strcpy(nomeSaida, "Nenhum arquivo aberto");
            }
            break;

        case 2: //Editar existente
            printf("\nDigite o nome do arquivo existente: ");
            scanf("%s", nomeSaida);
            arquivoAberto = 1;
            break;

        case 3: //Inserir
            if(solictaInserir(nomeSaida)) arquivoAberto = 1;
            else{
                arquivoAberto = 0;
                strcpy(nomeSaida, "Nenhum arquivo aberto");
            }
            break;

        case 4: //Buscar
            if(solicitaBuscar(nomeSaida)) arquivoAberto = 1;
            else{
                arquivoAberto = 0;
                strcpy(nomeSaida, "Nenhum arquivo aberto");
            }
            break;

        case 5: //Listagem
            if(solicitaImprimir(nomeSaida)) arquivoAberto = 1;
            else{
                arquivoAberto = 0;
                strcpy(nomeSaida, "Nenhum arquivo aberto");
            }
            break;

        default:
            printf("\nERRO: Opcao \"%d\" invalida.", opcao);
            break;
        }
    }

    return 0;
}
