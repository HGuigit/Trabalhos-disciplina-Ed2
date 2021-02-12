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
        int  dir;                   // Ponteiro a direita da casa do vetor de structs
        int  esq;                   // Ponteiro a esquerda da casa do vetor de structs
        int offset_arqPrincipal;     // Offset no arquivo de registros principal

}casa_nodo;

typedef struct{ // Cada nodo da Árvore B;
        casa_nodo no[4];
        int counter;

}nodo;


inserir abrirpAdd(FILE *arqREG)//abre arquivo para add
{
    int header=0;
    inserir input;
    int contador=0;
    //Lendo header do arquivo principal
    rewind(arqREG);
    fread(&header,sizeof(int),1,arqREG);
    //Abrindo arquivo de inseridos
    FILE *arq=fopen("insere.bin", "r+b");
    //Buscando onde parou de inserir
    fseek(arq,(header)*sizeof(inserir), SEEK_SET);
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
void imprimir(FILE *arq)
{
    int percorrer=1,lixo=0;
    inserir ler;
    percorrer=fread(&lixo,sizeof(int),1,arq);
    percorrer=fread(&ler,sizeof(inserir),1,arq);
    while(percorrer!=0)
    {
        printf("ID: %s\nDISC: %s\nNome: %s\nNome Disc: %s\nNota1:%.2f\nFreq:%.2f\n ",ler.id_aluno,ler.sigla_disc,ler.nome_aluno,ler.nome_disc,ler.media,ler.freq);
        percorrer=fread(&ler,sizeof(inserir),1,arq);
    }
}

int getBtree(FILE *arqARVB){
    int raizExiste; 
    rewind(arqARVB);
    fread(&raizExiste, sizeof(int), 1 , arqARVB);
    return raizExiste;
}

int buscar(inserir add, int raiz,FILE *arqARVB)
{
    nodo test;
    int qnt,counter=0;
    fseek(arqARVB,raiz,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    while(counter<qnt)
    {
        if((strcmp(test.no[counter].id_aluno,add.id_aluno) == 0)&&(strcmp(test.no[counter].sigla_disc,add.sigla_disc) == 0))
        {
            return 1;  
        }
        if((strcmp(test.no[counter].id_aluno,add.id_aluno) > 0)||(strcmp(test.no[counter].sigla_disc,add.sigla_disc) > 0))
        {
            counter++;    
        }
        else{
            if(test.no[counter].esq==-1)
            {
                printf("Não achado\n");
                return 0;
            }
            return(buscar(add,test.no[counter].esq,arqARVB));
        }
    }
    if(test.no[counter-1].dir==-1)
    {
        
        printf("Não achado\n");
        return 0;
    }
    return (buscar(add,test.no[counter-1].dir,arqARVB));
}



void criarArvore(int posi,inserir add,FILE *arqARVB){
    int offsetRoot;
    int counter = 1;

    nodo root;
    fseek(arqARVB, 0, SEEK_END);
    offsetRoot = ftell(arqARVB);
    strcpy(root.no[0].id_aluno, add.id_aluno);
    strcpy(root.no[0].sigla_disc, add.sigla_disc);
    root.no[0].offset_arqPrincipal = posi;
    root.no[0].dir=-1;
    root.no[0].esq=-1;
    fwrite(&root,sizeof(nodo),1,arqARVB);
    fwrite(&counter,sizeof(int),1,arqARVB);
    rewind(arqARVB);
    fwrite(&offsetRoot,sizeof(int),1,arqARVB);

}

int adiciona(inserir add, FILE *arqREG, FILE *arqARVB)
{
    int percorrer=1,header=0;
    int offset;
    inserir test;

    rewind(arqREG);
    
    if(getBtree(arqARVB) != -1){

         // Código de inserção de elemento na árvore
        if((buscar(add,getBtree(arqARVB), arqARVB)) != 1 )
        {
            fread(&header,sizeof(int),1,arqREG);
            header++;
            fseek(arqREG,0,SEEK_END);
            offset = ftell(arqREG);
            fwrite(&add,sizeof(inserir),1,arqREG);
            rewind(arqREG);
            fwrite(&header,sizeof(int),1,arqREG);
        }
        else{
            printf("Eh repetido\n");
        }

    }else{
        fread(&header,sizeof(int),1,arqREG);
        header++;
        fseek(arqREG,0,SEEK_END);
        offset = ftell(arqREG);
        fwrite(&add,sizeof(inserir),1,arqREG);
        rewind(arqREG);
        fwrite(&header,sizeof(int),1,arqREG);
        criarArvore(offset, add, arqARVB);
        



        
    }
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
	do{
		printf("Digite a opcao:\nAdicionar-->1\nPrintar alunos-->2\nBusca-->3\nSair-->4\n");
		scanf("%d",&opc);
		switch(opc)
		{
			case 1:	
				add=abrirpAdd(arqREG);
                header=adiciona(add,arqREG, arqARVB);
                break;
			case 2:
                imprimir(arqREG);
				break;
			case 3:
				break;
			case 4:
				return 0;				
		}
	}while(rep==1);
}