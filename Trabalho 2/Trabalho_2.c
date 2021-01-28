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

typedef struct key {
    char id_aluno[8];
    char disc[4];
    int  offset;
} CHAVE;




// ordena o vetor na memória e o insere no arquivo.
void ordena_vetor_insere(FILE *arqKEYS, CHAVE *vector_keys){

int tam;
int i, j;
int comparacao;
int comparacao2;
CHAVE aux;
char string_insere[16];
int tam_string_insere;

fseek(arqKEYS, sizeof(int), SEEK_SET);
fread(&tam, sizeof(int), 1, arqKEYS);
fseek(arqKEYS, 0, SEEK_SET);

for(i = 0; i < tam; i++){

    for( j = 0 ; j < tam-1 ; j++){
        comparacao = strcmp(vector_keys[j].id_aluno, vector_keys[j+1].id_aluno);

        
            if(comparacao > 0){
                aux = vector_keys[j];
                vector_keys[j] = vector_keys[j+1];
                vector_keys[j+1] = aux; 

            }
        }
    }

for(i = 0; i < tam; i++){

    for( j = 0 ; j < tam-1 ; j++){
        comparacao = strcmp(vector_keys[j].id_aluno, vector_keys[j+1].id_aluno);
            if(comparacao == 0){

            comparacao2 = strcmp(vector_keys[j].disc, vector_keys[j+1].disc);
            if(comparacao2 > 0){
                aux = vector_keys[j];
                vector_keys[j] = vector_keys[j+1];
                vector_keys[j+1] = aux; 

            

            }


            }
        
           
        }
    }



fseek(arqKEYS, 2*sizeof(int), SEEK_SET);
for( i = 0; i < tam; i++){

printf("\n%d:\n %s\n%s\n%d\n\n",i, vector_keys[i].id_aluno, vector_keys[i].disc, vector_keys[i].offset);
fwrite(vector_keys[i].id_aluno, 8*sizeof(char), 1 , arqKEYS);
fwrite(vector_keys[i].disc, 4*sizeof(char), 1 , arqKEYS);
fwrite(&vector_keys[i].offset, sizeof(int), 1 , arqKEYS);


}









}



// PAREI NA BUSCA
void busca_chave_valor_primaria(FILE *arqREG, FILE *arqKEYS, FILE* arqBUSCP, CHAVE *vector_keys){
    char id[8];
    char disc[4];
    char id2[8];
    char disc2[4];
    char buffer[100]; 
    int offset;
    int tam;
    int tam_ids;
    int i =0 ;
    int achou = 0;
    int count = 0;
    char *pch;
    fseek(arqBUSCP, 0, SEEK_END);
    fseek(arqKEYS, 0, SEEK_END );
    fseek(arqREG, 0, SEEK_SET);
    int fim = ftell(arqBUSCP);
    int fim_2 = ftell(arqKEYS);
    fseek(arqBUSCP, 0, SEEK_SET);
    fseek(arqKEYS, sizeof(int), SEEK_SET );
    fread(&tam_ids, sizeof(int), 1, arqKEYS);
    while(ftell(arqBUSCP) < fim){
        i = 0;
        achou = 0;
        fread(id, 8*sizeof(char), 1, arqBUSCP);
        fread(disc, 4*sizeof(char), 1, arqBUSCP);
        //printf("\n\n\n%s\n", id);
        //printf("%s\n------------------\n\n", disc);
        while(achou != 1 && i< tam_ids){
            strcpy(id2, vector_keys[i].id_aluno);
            strcpy(disc2,  vector_keys[i].disc);
            disc2[4] = '\0';
            if ( strcmp(id, id2) == 0 && strcmp(disc, disc2) == 0){
                fseek(arqREG, vector_keys[i].offset, SEEK_SET);
                fread(&tam, sizeof(int), 1, arqREG);
                fread(buffer, sizeof(char), tam, arqREG);
                buffer[tam] = '\0';
                pch = strtok(buffer, "#");
                    while(pch != NULL){
                        printf("%s\n", pch);
                        pch = strtok(NULL, "#");
                        count++;
                        if(count == 6){
                            count = 0;
                            printf("\n\n");
                            break;
                        }
                    }

                fseek(arqREG, 0, SEEK_SET);
                achou = 1;
            }
            i++;
        }


    }



    fseek(arqKEYS, 0, SEEK_SET);
    fseek(arqREG, 0, SEEK_SET);
    fseek(arqBUSCP, 0, SEEK_SET);


}







// Pega o vetor do arquivo
void get_Vector(FILE *arqREG, FILE *arqKEYS, CHAVE *vector_keys){
    int tam;
    int i = 0;
    int atualizado;
    char id[8];
    char disc[4];
    int offset;
    int tamREG;


    fread(&atualizado, sizeof(int), 1, arqKEYS);
    fread(&tam, sizeof(int), 1, arqKEYS);
    if(atualizado == 1){ 

    if(tam > 0 ){
        while( i < tam){
            fread(vector_keys[i].id_aluno, 8*sizeof(char), 1, arqKEYS);
            fread(vector_keys[i].disc, 4*sizeof(char), 1,arqKEYS);
            fread(&vector_keys[i].offset, sizeof(int), 1, arqKEYS);
            
            printf("\n%s\n%s\n%d\n\n", vector_keys[i].id_aluno, vector_keys[i].disc, vector_keys[i].offset);
            system("pause");
            i++;

        }
        i=0;
        
    }
    fseek(arqKEYS, 0, SEEK_SET);
    }else if(atualizado == 0){
        fseek(arqREG, sizeof(int), SEEK_SET);
        while(i < tam){

            offset = ftell(arqREG);
            fread(&tamREG, sizeof(int), 1, arqREG);
            fread(id, 8*(sizeof(char)), 1, arqREG);
            fread(disc, 4*(sizeof(char)),1, arqREG);
            fseek(arqREG, tamREG - 12, SEEK_CUR);
            id[8] = '\0';
            disc[4] = '\0';
            strcpy(vector_keys[i].id_aluno, id);
            strcpy(vector_keys[i].disc, disc);
            vector_keys[i].offset = offset;
            i++;
            



        }

    i=0;
    
    }
    fseek(arqREG, 0, SEEK_SET);
    

}

// Atualiza o vetor na memória na hora de inserir algum registro
void fetch_Vector(FILE *arqKEYS, char *id, char *disc, int offset, CHAVE *vector_keys){
    CHAVE insere_vetor;
    int num_keys;

    strcpy(insere_vetor.id_aluno, id);
    strcpy(insere_vetor.disc, disc);
    insere_vetor.offset = offset;

    fseek(arqKEYS, sizeof(int), SEEK_SET);
    fread(&num_keys, sizeof(int), 1, arqKEYS);
    fseek(arqKEYS, sizeof(int), SEEK_SET);
    vector_keys[num_keys] = insere_vetor;
    num_keys = num_keys + 1;
    fwrite(&num_keys, sizeof(int), 1, arqKEYS);
    fseek(arqKEYS, 0, SEEK_SET);



}







void Insere_Registro_caso_2(FILE *arqIN, FILE *arqREG, FILE *arqKEYS, CHAVE *vector_keys){

ALUNO registro;
char registro_buffer[150];
int tamArq;
int PosParouDeLer; // Onde parei de ler na inserção ou num de registros no arquivo
int tam;
int offset;

// Pegando tam do arquivo
fseek(arqIN, 0, SEEK_END);
tamArq = ftell(arqIN);
fseek(arqIN, 0, SEEK_SET);

// Lendo posição onde parou de inserir da header
fseek(arqREG, 0, SEEK_SET);
fread(&PosParouDeLer, sizeof(int), 1, arqREG);
fseek(arqIN, sizeof(ALUNO)*PosParouDeLer, SEEK_SET);
if(ftell(arqIN) >= tamArq){

    printf("\n\nNao ha mais nada a ser inserido!\n");

}else{
PosParouDeLer = PosParouDeLer + 1;
fseek(arqREG, 0, SEEK_SET);
fwrite(&PosParouDeLer, sizeof(int), 1, arqREG);

//Bufferizando registro
fread(&registro , sizeof(ALUNO), 1, arqIN);
sprintf(registro_buffer, "%s#%s#%s#%s#%.1f#%.1f#",registro.id_aluno,registro.sigla_disc, registro.nome_aluno, registro.nome_disc, registro.media, registro.freq);
tam = strlen(registro_buffer);

//Inserindo arqREG

fseek(arqREG, 0, SEEK_END);
offset = ftell(arqREG);
fwrite(&tam, sizeof(int), 1, arqREG);
fwrite(registro_buffer, sizeof(char), tam, arqREG);


fetch_Vector(arqKEYS, registro.id_aluno, registro.sigla_disc, offset, vector_keys);




}



}





int main(void){

FILE *arqIN;
FILE *arqREG;
FILE *arqKEYS;
FILE *arqBUSCP;

CHAVE vector_keys[200];

int nao_atualizado = 0;
int atualizado = 1;
int num_keys = 0;






    
int sair=0;




while(sair != 5){

system("cls");

printf("                  ---- ---- MENU ---- ----           \n\n");
printf("1  -  Carregar arquivos \n");
printf("2  -  Inserir um Registro \n");
printf("3  -  Busca Primária \n");
printf("4  -  ?\n");
printf("5  -  Sair              \n\n");

scanf("%d", &sair);







switch (sair){

case 1:


    if((arqBUSCP = fopen("busca_p.bin", "r+b")) == NULL){
        printf("\nErro na leitura do arquivo de busca primária.");
        exit(0);
    }

    if((arqIN = fopen("insere.bin", "r+b")) == NULL){
            printf("\nErro na leitura do arquivo de insercao.");
            exit(0);
    }

    if((arqREG = fopen("registros.bin", "r+b")) == NULL){
        int initPegaRegistro = 0; // Onde parei de ler no arquivo de inserÃ§Ã£o
        system("cls");
        printf("\n Arquivo de saida nao existente, criando um novo...\n ");
        Sleep(1000);
        if((arqREG = fopen("registros.bin", "w+b")) == NULL){
            printf("\nFalha na criacao do arquivo.");
        }else{
            fwrite(&initPegaRegistro, sizeof(int), 1, arqREG);
        }
    }else{
        system("cls");
        printf("\n Arquivo de registros carregado com sucesso !\n ");
        Sleep(500);
    }

    if((arqKEYS = fopen("primary_keys.bin", "r+b")) == NULL){
        system("cls");
        printf("\n Arquivo de chaves primárias não existente, criando um novo...\n ");
        Sleep(1000);
        if((arqKEYS = fopen("primary_keys.bin", "w+b")) == NULL){
            printf("\nFalha na criacao do arquivo.");
        }else{
            fwrite(&nao_atualizado, sizeof(int), 1 ,arqKEYS);
            fwrite(&num_keys, sizeof(int), 1, arqKEYS);
            get_Vector(arqREG ,arqKEYS, vector_keys);
            fseek(arqKEYS, 0, SEEK_SET);
        }

    }else{

        get_Vector(arqREG, arqKEYS, vector_keys);
        system("cls");
        fwrite(&nao_atualizado, sizeof(int), 1 ,arqKEYS);
        fseek(arqKEYS, 0, SEEK_SET);
        printf("\n Arquivo de chaves primarias carregado com sucesso !\n ");
        Sleep(500);

    }
    break;

case 2: 
    Insere_Registro_caso_2(arqIN, arqREG, arqKEYS, vector_keys);
    Sleep(1000);


    break;

case 3:
    busca_chave_valor_primaria(arqREG, arqKEYS, arqBUSCP, vector_keys);
    system("pause");
    break;


case 5: 
    ordena_vetor_insere(arqKEYS, vector_keys);
    fseek(arqKEYS, 0, SEEK_SET);
    fwrite(&atualizado, sizeof(int), 1, arqKEYS);
    printf("\n\nSaindo...\n\n");
    fclose(arqKEYS);
    fclose(arqREG);
    fclose(arqIN);
    break;

default:
    printf("\n\nEntrada Invalida... :?\n\n");
    Sleep(1000);
    system("cls");

    break;
}




}




}












