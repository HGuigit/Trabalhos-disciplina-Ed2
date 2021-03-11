#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#define TAMANHO 13

typedef struct{//para inserir
        char id_aluno[8];
        char sigla_disc[4];
        char nome_aluno[50];
        char nome_disc[50];
        float media;
        float freq;
    }inserir;

typedef struct{
    int flag;
    int hash;
    int posi;
}indice;

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





int testeExiste(inserir add,FILE *arqHASH)
{
    int numID = (atoi(add.id_aluno))*1000;
    int numDISC = atoi(add.sigla_disc);
    int posi=0,posi_inicial=0,hash,tentativas=0;
    indice teste;
    hash=numID+numDISC;
    posi = hash % TAMANHO;
    posi_inicial = posi;
    do{
        fseek(arqHASH,posi*sizeof(indice),SEEK_SET);
        fread(&teste, sizeof(indice), 1, arqHASH);
        if(teste.flag==0)
        {
        	printf("inserido com %d tentativas na posicao %d\n",tentativas,posi);
            return posi;
        }else if(teste.hash == hash){
            return  -1;
        }else if(teste.flag == 1){
             if(posi == TAMANHO-1){
             	tentativas++;
                posi = 0;
            }
            else{
            	tentativas++;
				posi++;
			}
        }
       
    }while(posi_inicial!=posi);
    return -2;
}




void adiciona(FILE *arqHASH,FILE *arqREG,inserir add)
{
    int numID = (atoi(add.id_aluno))*1000;
    int numDISC = atoi(add.sigla_disc);
    int resto=0,tam=13,hash,test=testeExiste(add,arqHASH),qnt=0;
    if(test==-1)
    {
        printf("\nRepetido!\n");
        return;
    }
    else if(test==-2)
    {
        printf("Indice cheio\n");
        return;
    }
    fread(&qnt,sizeof(int),1,arqREG);
    fseek(arqREG,0,SEEK_END);
    indice hash_row;
    hash=numID+numDISC;
    hash_row.posi=ftell(arqREG);
    hash_row.hash=hash;
    hash_row.flag=1;
    fwrite(&add,sizeof(inserir),1,arqREG);
    rewind(arqREG);
    qnt++;
    fwrite(&qnt,sizeof(int),1,arqREG);
    fseek(arqHASH,test*sizeof(indice),SEEK_SET);
    fwrite(&hash_row,sizeof(indice),1,arqHASH);


}



int main(){

    FILE *arqREG;
    FILE *arqHASH;
    FILE *arqINPUT;
    FILE *arqREM;
    FILE *arqBUSCA;
    indice aux;
    inserir add;
    int rep = 1,numInseridos=0,opc=0;


    if((arqHASH = fopen("Hash.bin", "r+b")) == NULL){
        
        system("cls");
        printf("\nArquivo de HASH nao existente, criando um novo arquivo...\n");

        if((arqHASH = fopen("Hash.bin", "w+b")) == NULL){
            printf("\nFalha em criar arquivo de HASH...\n");
            Sleep(1500);
            system("cls");

        }else{
            aux.flag=0;
            aux.hash=0;
            aux.posi=0;
            rep=fwrite(&aux, sizeof(indice), 13, arqHASH);
            fseek(arqHASH, 0, SEEK_SET);
            printf("\nSucesso em criar o arquivo de HASH...\n");
            Sleep(1500);
            system("cls");
        }

    }else{
        system("cls");
        printf("\nArquivo de HASH aberto com sucesso...\n");
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
            fwrite(&numInseridos, sizeof(int), 1, arqREG);
            fseek(arqREG, 0, SEEK_SET);
            printf("\nSucesso em criar o arquivo de Registros...\n");
            Sleep(1500);
            system("cls");
        }

    }else{
        printf("\nArquivo de Registros aberto com sucesso...\n");
        system("cls");
        Sleep(1500);

    }


    do{
		printf("Digite a opcao:\nAdicionar-->1\nRemover-->2\nBuscar-->3\nSair-->4\n");
		scanf("%d",&opc);
		switch(opc)
		{
			case 1:	
                add=abrirpAdd(arqREG);
                adiciona(arqHASH, arqREG, add);
                break;
			case 2:
               
				break;
			case 3:
               
				break;
			case 4:
				fclose(arqHASH);
				fclose(arqREG);
				return 0;				
		}
    }while(rep);

}

