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

void Remove_Registro(FILE *arqOUT, FILE *arqREMOV){

char id[7];
char disc[3];
char idarqOUT[7];
char discarqOUT[3];
char charRemov = ' ';
int point_count = 0;
int i=0;
int j=0;
int pareiDeLer; // numero de registros q ja li
int acabouArq = 0; 
int tam;
int achou = 0;

char marcador = '*';
int offset_final = -1;
int posUltimoEle;
int elemLista;

id[7] = '\0';
disc[3] = '\0';
idarqOUT[7] = '\0';
discarqOUT[3] = '\0';

fseek(arqOUT, 2*sizeof(int), SEEK_SET); //Pega onde parei de remover no arquivo de remoção(numero gravado em arqOUT, terceiro inteiro no começo)
fread(&pareiDeLer, sizeof(int),1,arqOUT); 
fseek(arqOUT, -4, SEEK_CUR); //Posiciona ponteiro para sobrescrever o campo adicionando 1

fseek(arqREMOV, 12*pareiDeLer, SEEK_SET);

// Pegar ID e Disciplina a serem Removidas

while(point_count != 2){

charRemov = fgetc(arqREMOV);
if(feof(arqREMOV) != 0){
    printf("\nNao ha mais registros para remover!\n, abortando...");
    acabouArq = 1;
    break;
    
}
if(charRemov == 0){
    point_count++;
    continue;
}

if(point_count == 0){
id[i] = charRemov;
i++;
}
if(point_count == 1){
disc[j] = charRemov;
j++;
}



}

if(acabouArq == 0){
    printf("Removendo registro:\n\n");
    printf("id:\n%s\n\n", id);
    printf("disc:\n%s\n", disc);
    ///////

    //Incrementar 1 no terceiro inteiro de arqOUT(Onde parei de remover)

    pareiDeLer = pareiDeLer + 1;
    fwrite(&pareiDeLer, sizeof(int), 1,arqOUT);
    fseek(arqOUT, 0, SEEK_SET);
    /////
    fseek(arqOUT, 12, SEEK_SET);

while(achou == 0){
    fread(&tam, sizeof(int), 1, arqOUT);
    fread(&idarqOUT, sizeof(char), 7, arqOUT);
    
    fseek(arqOUT, 1, SEEK_CUR);
    fread(&discarqOUT, sizeof(char), 3, arqOUT);
    if((strcmp(id, idarqOUT) == 0) && (strcmp(disc, discarqOUT) == 0)){
        fseek(arqOUT, -15, SEEK_CUR);
        posUltimoEle = ftell(arqOUT);
        fseek(arqOUT, 4, SEEK_CUR);
        achou = 1;
        fwrite(&marcador, sizeof(char), 1, arqOUT);
        fwrite(&offset_final, sizeof(int), 1, arqOUT);
        break;
    }

    tam = tam - 15;
    fseek(arqOUT, tam+4, SEEK_CUR);
}
fseek(arqOUT, 0, SEEK_SET);
fread(&elemLista, sizeof(int), 1, arqOUT);
achou = 0;
if(elemLista == -1){
    fseek(arqOUT, 0, SEEK_SET);
    elemLista = posUltimoEle;
    fwrite(&elemLista, sizeof(int), 1, arqOUT);
}else{
    while(achou == 0){
    fseek(arqOUT, elemLista + 5, SEEK_SET);
    fread(&elemLista, sizeof(int), 1, arqOUT);
    if(elemLista == -1){
        fseek(arqOUT, -4, SEEK_CUR);
        elemLista = posUltimoEle;
        fwrite(&elemLista, sizeof(int), 1, arqOUT);
        break;
      
    }
    }
}






}



}

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
        int initRemoveRegistro = 0; // Onde parei de remover a partir do arquivo de remoção
        system("cls");
        printf("\n Arquivo de saida nao existente, criando um novo...\n ");
        Sleep(1000);
        if((arqOUT = fopen("registros.bin", "w+b")) == NULL){
            printf("\nFalha na criação do arquivo.");
        }else{
            fwrite(&init ,sizeof(int), 1 ,arqOUT);
            fwrite(&initPegaRegistro, sizeof(int), 1, arqOUT);
            fwrite(&initRemoveRegistro, sizeof(int),1, arqOUT);
        }
    }else{
        system("cls");
        printf("\n Arquivos carregados com sucesso !\n ");
        Sleep(500);
    }



    break;
case 2:
    Insere_Registro(arqOUT, arqIN);
    Sleep(1000);


    break;

case 3:
    Remove_Registro(arqOUT, arqREMOV);
    Sleep(1500);

    break;

case 4:


    break;

case 5:
    printf("\n\nSaindo... :(\n\n");
    Sleep(500);
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

