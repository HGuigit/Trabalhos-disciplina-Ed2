#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>











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
        system("cls");
        printf("\n Arquivo de saida nao existente, criando um novo...\n ");
        Sleep(1000);
        if((arqOUT = fopen("registros.bin", "w+b")) == NULL){
            printf("\nFalha na criação do arquivo.");
        }
    }else{
        system("cls");
        printf("\n Arquivos carregados com sucesso !\n ");
        Sleep(500);
    }



    break;
case 2:

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

