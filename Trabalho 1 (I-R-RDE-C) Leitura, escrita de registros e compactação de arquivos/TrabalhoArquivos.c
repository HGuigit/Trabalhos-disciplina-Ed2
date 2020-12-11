#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct hist {
        char id_aluno[8];
        char sigla_disc[4];
        char nome_aluno[50];
        char nome_disc[50];
        float media;
        float freq;
    } ALUNO;










void Insere_Registro(FILE *arqOUT, FILE *arqIN){

ALUNO registro;
char registro_buffer[150];
int tam;
int initEle;
int PosParouDeLer;

//Pegando posição onde parei de ler e ajustando onde devo ler no arquivo de inserção(Essa posição é gravado no arquivo OUT após o primeiro elemento da lista ligada)
fseek(arqOUT, sizeof(int), SEEK_SET);
fread(&PosParouDeLer, sizeof(int), 1, arqOUT);
fseek(arqIN, sizeof(ALUNO)*PosParouDeLer, SEEK_SET);
PosParouDeLer = PosParouDeLer + 1;
fseek(arqOUT, sizeof(int), SEEK_SET);
fwrite(&PosParouDeLer, sizeof(int), 1,arqOUT);



//Bufferizando dado do registro
fread(&registro , sizeof(ALUNO), 1, arqIN);
sprintf(registro_buffer, "%s#%s#%s#%s#%.1f#%.1f",registro.id_aluno,registro.sigla_disc, registro.nome_aluno, registro.nome_disc, registro.media, registro.freq);
tam = strlen(registro_buffer);



fseek(arqOUT, 0, SEEK_SET);
fread(&initEle,sizeof(int),1,arqOUT);
if(initEle == -1){
fseek(arqOUT, 0, SEEK_END);    
fwrite(&tam, sizeof(int), 1, arqOUT);
fwrite(registro_buffer,sizeof(char), tam ,arqOUT);
printf("\nRegistro inserido no final do arquivo!\n\n");
Sleep(1000);
}
















};








int main(void){

FILE *arqIN;
FILE *arqOUT;
FILE *arqREMOV;

int sair=0;


while(sair != 5){

system("cls");

printf("                  ---- ---- MENU ---- ----           \n\n");
printf("1  -  Carregar arquivos \n");
printf("2  -  Inserir um Registro \n");
printf("3  -  Retirar um registro \n");
printf("4  -  Compactar arquivo\n");
printf("5  -  Sair              \n\n");

scanf("%d", &sair);

switch (sair){
case 1:

    if((arqIN = fopen("insere.bin", "r+b")) == NULL){
        printf("\nErro na leitura do arquivo de inserção.");
        exit(0);
    }

    if((arqREMOV = fopen("remove.bin", "r+b")) == NULL){
        printf("\nErro na leitura do arquivo de remoção.");
        exit(0);
    }
    if((arqOUT = fopen("registros.bin", "r+b")) == NULL){
        int init = -1; //inicio lista ligada
        int initPegaRegistro = 0; // Onde parei de ler no arquivo de inserção
        system("cls");
        printf("\n Arquivo de saida nao existente, criando um novo...\n ");
        Sleep(1000);
        if((arqOUT = fopen("registros.bin", "w+b")) == NULL){
            printf("\nFalha na criação do arquivo.");
        }else{
            fwrite(&init ,sizeof(int), 1 ,arqOUT);
            fwrite(&initPegaRegistro, sizeof(int), 1, arqOUT);
        }
    }else{
        system("cls");
        printf("\n Arquivos carregados com sucesso !\n ");
        Sleep(500);
    }



    break;
case 2:
    Insere_Registro(arqOUT, arqIN);
    Sleep(2000);


    break;

case 3:


    break;

case 4:


    break;

case 5:
    printf("\n\nSaindo... :(\n\n");
    Sleep(1000);
    fclose(arqIN);
    fclose(arqOUT);
    fclose(arqREMOV);
    break;
default:
    printf("\n\nEntrada Invalida... :?\n\n");
    Sleep(1000);
    system("cls");

    break;
}






}























return 0;
}

