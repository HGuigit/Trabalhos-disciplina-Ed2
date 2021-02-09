#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <time.h>



typedef struct{//para inserir
        char id_aluno[8];
        char sigla_disc[4];
        char nome_aluno[50];
        char nome_disc[50];
        float media;
        float freq;
    }inserir;

typedef struct{//para ler as pessoas para buscar
        char id_aluno[8];
        char sigla_disc[4];
	}busca;
    
typedef struct{                      //para criar uma "casa" do nodo da árvore B
        char id_aluno[8];
        char sigla_disc[4];
        int  *dir;                   // Ponteiro a direita da casa do vetor de structs
        int  *esq;                   // Ponteiro a esquerda da casa do vetor de structs
        int offset_arqPrincipal;     // Offset no arquivo de registros principal

}casa_nodo;

typedef struct{ // Cada nodo da Árvore B;
        casa_nodo no[3];
}nodo;


inserir abrirpAdd(FILE *arqREG)//abre arquivo para add
{
    int header=0;
    inserir input;
    int contador=0;
    //Lendo header do arquivo principal

    fread(&header,sizeof(int),1,arqREG);
    //Abrindo arquivo de inseridos
    FILE *arq=fopen("insere.bin", "r+b");
    //Buscando onde parou de inserir
    fseek(arq,header*sizeof(inserir), SEEK_SET);
    fread(&input,sizeof(inserir),1,arq);
    fclose(arq);
    rewind(arqREG);
    return(input);  
}

busca abrirB()//abre arquivo para busca
{
    FILE *arq=fopen("busca.bin", "rb");
    busca input;
    fread(&input,sizeof(busca),1,arq);
    fclose(arq);
	return input;
}

nodo criarArvore(){
    nodo root;

    





}

void criarArquivos(FILE *arqREG, FILE *arqARVB){

    //Headers arqARVB
    int naoExisteArv = -1;

    //Headers arqREG
    int numInseridos = 0;



    if((arqARVB = fopen("ArvoreB.bin", "r+b")) == NULL){
        
        system("cls");
        printf("\nArquivo de arvore-B nao existente, criando uma nova raiz em um novo arquivo...\n");

        if((arqARVB = fopen("ArvoreB.bin", "w+b")) == NULL){
            printf("\nFalha em criar arquivo de arvore-B...\n");
            Sleep(1500);
            system("cls");

        }else{


            printf("\nSucesso em criar o arquivo de arvore-B...\n");
            Sleep(1500);
            system("cls");
            fwrite(&naoExisteArv, sizeof(int), 1, arqARVB);
            fseek(arqARVB, 0, SEEK_SET);
        }

    }else{
        system("cls");
        printf("\nArquivo de arvore-B aberto com sucesso...\n");
        Sleep(1500);

    }

    if((arqREG = fopen("registros.bin", "r+b")) == NULL){
        
        system("cls");
        printf("\nArquivo de Registros principal nao existente, criando um novo...\n");

        if((arqREG = fopen("registros.bin", "w+b")) == NULL){
            printf("\nFalha em criar arquivo de Registros...\n");
            Sleep(1500);
            system("cls");

        }else{


            printf("\nSucesso em criar o arquivo de Registros...\n");
            Sleep(1500);
            system("cls");
            fwrite(&numInseridos, sizeof(int), 1, arqREG);
            fseek(arqREG, 0, SEEK_SET);
        }

    }else{
        printf("\nArquivo de Registros aberto com sucesso...\n");
        system("cls");
        Sleep(1500);

    }
}
int adiciona(inserir add, FILE *arqREG)
{
    int percorrer=1,header=0;
    inserir test;
    rewind(arqREG);
    fread(&header,sizeof(int),1,arqREG);
    while(percorrer!=0)
    {
        percorrer=fread(&test,sizeof(inserir),1,arqREG);
        if((strcmp(test.id_aluno,add.id_aluno)!=0)&&(strcmp(test.sigla_disc,add.sigla_disc)!=0))
        {
            printf("Aluno ja adicionado!\n");
            return -1;
        }
    }
    header++;
    fwrite(&test,sizeof(inserir),1,arqREG);
    rewind(arqREG);
    fwrite(&header,sizeof(int),1,arqREG);
    return header;
}

int main()
{

    busca procura;
    inserir add;
    int opc=0,rep=1, header = 0;
    nodo root;

    //Arquivos do programa
    FILE *arqARVB;
    FILE *arqREG;
    criarArquivos(arqREG, arqARVB);
	do{
		printf("Digite a opcao:\nAdicionar-->1\nPrintar alunos-->2\nBusca-->3\nSair-->4\n");
		scanf("%d",&opc);
		switch(opc)
		{
			case 1:	
				add=abrirpAdd(arqREG);
                header=adiciona(add,arqREG);
                break;
			case 2:
                procura=abrirB();
				break;
			case 3:
				break;
			case 4:
				return 0;				
		}
	}while(rep==1);
}