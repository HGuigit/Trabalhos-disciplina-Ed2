#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


// PAREI AO JOGAR OS NOMES PRO VETOR SECUNDÁRIO EM MEMÓRIA


typedef struct chaves{

char id[8];
char disc[4];
int prox;

}CHAVE_SEC;










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
    int  prox;
} CHAVE;

typedef struct nomes{
int tam_vector;
char nome[50];
int offset_primeiro_indice;
CHAVE indices[15];


}INDICE_NOMES;




// ordena o vetor na memória e o insere no arquivo.
void ordena_vetor_insere(FILE *arqKEYS, CHAVE *vector_keys, FILE *arqSECNAMES, FILE*arqSECKEYS, INDICE_NOMES *vector_nomes){

int tam;
int tam_nomes;
int i, j;
int comparacao;
int comparacao2;
CHAVE aux;
INDICE_NOMES aux2;
char string_insere[16];
int tam_string_insere;
char nome_bufferizado[50];
char end_of_string = '\0';
char marker = '#';
int tam_chaves_nome;
char ch;
int count = 0;
int prox;

fseek(arqKEYS, sizeof(int), SEEK_SET);
fread(&tam, sizeof(int), 1, arqKEYS);
fseek(arqKEYS, 0, SEEK_SET);


// Odernação busca prim
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
// Ordenação busca prim
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

// Ordernação busca sec
fseek(arqSECNAMES, 0, SEEK_SET);
fread(&tam_nomes, sizeof(int), 1, arqSECNAMES);
fseek(arqSECNAMES, sizeof(int), SEEK_CUR);
for(i = 0; i < tam_nomes; i++){
     for( j = 0 ; j < tam_nomes -1  ; j++){
        comparacao = strcmp(vector_nomes[j].nome, vector_nomes[j+1].nome);

        
            if(comparacao > 0){
                aux2 = vector_nomes[j];
                vector_nomes[j] = vector_nomes[j+1];
                vector_nomes[j+1] = aux2; 

            }
        }
}

// Ordenação chaves secundárias;
fseek(arqSECNAMES, 0, SEEK_SET);
fseek(arqSECNAMES, 3*sizeof(int), SEEK_SET);
while((ch = fgetc(arqSECNAMES)) != EOF){
   if(ch == '#'){
        fread(&tam_chaves_nome, sizeof(int), 1, arqSECNAMES);
        for(i = 0; i < tam_chaves_nome ; i++){
            for(j=0; j < tam_chaves_nome - 1; j++){

                comparacao = strcmp(vector_nomes[count].indices[j].disc,vector_nomes[count].indices[j+1].disc);

                if(comparacao > 0){
                    aux = vector_nomes[count].indices[j];
                    vector_nomes[count].indices[j] = vector_nomes[count].indices[j+1];
                    vector_nomes[count].indices[j+1] = aux;
                }
            }

        }



        count++;
   }
   

}





fseek(arqKEYS, 2*sizeof(int), SEEK_SET);
fseek(arqSECNAMES, 3*sizeof(int), SEEK_SET);

for( i = 0; i < tam; i++){

fwrite(vector_keys[i].id_aluno, 8*sizeof(char), 1 , arqKEYS);
fwrite(vector_keys[i].disc, 4*sizeof(char), 1 , arqKEYS);
fwrite(&vector_keys[i].offset, sizeof(int), 1 , arqKEYS);


}
fwrite(&marker, sizeof(marker), 1,arqSECNAMES);
fseek(arqSECKEYS, 0, SEEK_SET);
for(i = 0; i < tam_nomes; i++){
fwrite(&vector_nomes[i].tam_vector, sizeof(int), 1, arqSECNAMES);
sprintf(nome_bufferizado, "%s", vector_nomes[i].nome);
fwrite(nome_bufferizado, sizeof(char), strlen(nome_bufferizado), arqSECNAMES);
fwrite(&end_of_string, sizeof(end_of_string), 1, arqSECNAMES);
    for( j = 0; j < vector_nomes[i].tam_vector; j++){
        if(j == 0 && vector_nomes[i].tam_vector != 1){
            vector_nomes[i].indices[j].prox = -1;
            prox = ftell(arqSECKEYS);
            fwrite(vector_nomes[i].indices[j].id_aluno, 8*sizeof(char), 1, arqSECKEYS);
            fwrite(vector_nomes[i].indices[j].disc, 4*sizeof(char), 1, arqSECKEYS);
            fwrite(&vector_nomes[i].indices[j].prox,  sizeof(int), 1,arqSECKEYS);
        }else if(j == (vector_nomes[i].tam_vector - 1) && vector_nomes[i].tam_vector != 1){
            vector_nomes[i].offset_primeiro_indice = ftell(arqSECKEYS);
            fwrite(&vector_nomes[i].offset_primeiro_indice, sizeof(int), 1, arqSECNAMES);
            vector_nomes[i].indices[j].prox = prox;
            fwrite(vector_nomes[i].indices[j].id_aluno, 8*sizeof(char), 1, arqSECKEYS);
            fwrite(vector_nomes[i].indices[j].disc, 4*sizeof(char), 1, arqSECKEYS);
            fwrite(&vector_nomes[i].indices[j].prox, sizeof(int), 1, arqSECKEYS);
        }else if(j == 0 && vector_nomes[i].tam_vector == 1){
            vector_nomes[i].indices[j].prox = -1;
            vector_nomes[i].offset_primeiro_indice = ftell(arqSECKEYS);
            fwrite(&vector_nomes[i].offset_primeiro_indice, sizeof(int), 1, arqSECNAMES);
            fwrite(vector_nomes[i].indices[j].id_aluno, 8*sizeof(char), 1, arqSECKEYS);
            fwrite(vector_nomes[i].indices[j].disc, 4*sizeof(char), 1, arqSECKEYS);
            fwrite(&vector_nomes[i].indices[j].prox, sizeof(int), 1, arqSECKEYS);
        }
        else{
            vector_nomes[i].indices[j].prox = prox;
            prox = ftell(arqSECKEYS);
            fwrite(vector_nomes[i].indices[j].id_aluno, 8*sizeof(char), 1, arqSECKEYS);
            fwrite(vector_nomes[i].indices[j].disc, 4*sizeof(char), 1, arqSECKEYS);
            fwrite(&vector_nomes[i].indices[j].prox,  sizeof(int), 1,arqSECKEYS);
        }
        

        
    }



fwrite(&marker, sizeof(marker),1,arqSECNAMES);

}





}


void busca_chave_valor_secundaria(FILE *arqSECNAMES,  FILE *arqSECKEYS, FILE *arqREG, FILE *arqBUSCS, FILE *arqKEYS, CHAVE *vector_keys){

int num_nomes;
int  i = 0;
int  j = 0;
char ch;
int tam_vector_ids;
char nome_busca[50];
int tam_busca;
char nome[50];
int offset;
int achou = 0;
char id[8];
char disc[4];
int tam_arr_keys;
char *pch;
int tam_reg;
char buffer[100];
int count = 0;

fseek(arqSECNAMES, 0, SEEK_SET);
fread(&num_nomes, sizeof(int), 1, arqSECNAMES);
fseek(arqSECNAMES, 2*sizeof(int), SEEK_CUR);

fseek(arqBUSCS, 0, SEEK_END);
tam_busca = ftell(arqBUSCS);
fseek(arqBUSCS, 0, SEEK_SET);
fseek(arqKEYS, sizeof(int), SEEK_SET);
fread(&tam_arr_keys, sizeof(int), 1, arqKEYS);


while(ftell(arqBUSCS) < tam_busca){

fread(nome_busca, 50*sizeof(char), 1, arqBUSCS);
nome_busca[50] = '\0';
fseek(arqSECNAMES, 3*sizeof(int), SEEK_SET);

    while(i < num_nomes){

    ch = fgetc(arqSECNAMES);
   

    if(ch == '#'){
        fread(&tam_vector_ids, sizeof(int), 1, arqSECNAMES);

    }
    if(ch != '#'){
        strncat(nome, &ch, 1);

    }
    if(ch == '\0'){
       // printf("nome : %s\n", nome);
       // printf("nome_busca : %s\n", nome_busca);
        fread(&offset, sizeof(int), 1, arqSECNAMES);
        if(strcmp(nome, nome_busca) == 0){
        printf("\n\n\t\t-------------------- Registros com o nome --> %s -------------\n\n", nome_busca);

        while ( offset != -1){
            fseek(arqSECKEYS, offset, SEEK_SET);
            fread(id, 8*sizeof(char),1 , arqSECKEYS);
            fread(disc, 4*sizeof(char), 1, arqSECKEYS);
            fread(&offset, sizeof(int), 1 , arqSECKEYS);
            for(j = 0; j < tam_arr_keys ; j++){
                if(strcmp(vector_keys[j].id_aluno, id) == 0 && strcmp(vector_keys[j].disc, disc) == 0){
                    fseek(arqREG, vector_keys[j].offset, SEEK_SET);
                    fread(&tam_reg, sizeof(int), 1, arqREG);
                    fread(buffer, tam_reg*sizeof(char), 1, arqREG);
                    buffer[tam_reg] = '\0';
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





                }
            
            }
        

            
        }
        }

        i++;
        strcpy(nome, "");
       
    }



    }
    strcpy(nome_busca, "");
  i = 0;


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
    //printf("%d", tam);
    if(atualizado == 1){ 
        
    if(tam > 0 ){
        while( i < tam){

            fread(vector_keys[i].id_aluno, 8*sizeof(char), 1, arqKEYS);
            fread(vector_keys[i].disc, 4*sizeof(char), 1,arqKEYS);
            fread(&vector_keys[i].offset, sizeof(int), 1, arqKEYS);
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
            fread(id, sizeof(char), 7, arqREG);
            fseek(arqREG, 1, SEEK_CUR);
            fread(disc, sizeof(char),3, arqREG);
            fseek(arqREG, -11 , SEEK_CUR);
            fseek(arqREG, tamREG, SEEK_CUR);
            id[7] = '\0';
            disc[3] = '\0';
            strcpy(vector_keys[i].id_aluno, id);
            strcpy(vector_keys[i].disc, disc);
            vector_keys[i].offset = offset;
            //printf("\n\n%s",vector_keys[i].id_aluno);
            //printf("\n%s\n\n", vector_keys[i].disc);
            //system("pause");
            i++;
            



        }

    i=0;
    
    }
    fseek(arqREG, 0, SEEK_SET);
    

}


void get_vector_secondary(FILE *arqSECNAMES, FILE* arqSECKEYS, INDICE_NOMES *vector_nomes){

int num_nomes;
int i = 0;
int j = 0;
char ch;
int tam_vetor_ids;
char nome[50] = "";
int prox;

fseek(arqSECNAMES, 0, SEEK_SET);

fread(&num_nomes, sizeof(int), 1, arqSECNAMES);
fseek(arqSECNAMES, 2*sizeof(int), SEEK_CUR);


while(i < num_nomes){

    ch = fgetc(arqSECNAMES);
   
    if(ch == '#'){

        fread(&vector_nomes[i].tam_vector, sizeof(int), 1, arqSECNAMES);

    }
    if(ch != '#'){
    strncat(nome, &ch, 1);
    }
    if(ch == '\0'){
        strcpy(vector_nomes[i].nome, nome);
        fread(&vector_nomes[i].offset_primeiro_indice, sizeof(int), 1, arqSECNAMES);

        //printf("%d", vector_nomes[i].tam_vector);
        //printf("\n%s\n", vector_nomes[i].nome);
        //printf("%d\n\n", vector_nomes[i].offset_primeiro_indice);

        fseek(arqSECKEYS, vector_nomes[i].offset_primeiro_indice, SEEK_SET);
        for( j = 0; j < vector_nomes[i].tam_vector; j++){
            fread(vector_nomes[i].indices[j].id_aluno, 8*sizeof(char), 1, arqSECKEYS);
            fread(vector_nomes[i].indices[j].disc, 4*sizeof(char), 1, arqSECKEYS);
            fread(&prox, sizeof(int), 1, arqSECKEYS);
            //printf("\n%s\n", vector_nomes[i].indices[j].id_aluno);
            //printf("%s\n\n", vector_nomes[i].indices[j].disc);
            if(prox == -1){
                j=0;
                break;
            }else{
                fseek(arqSECKEYS, prox, SEEK_SET);
            }
            


            
        }

        strcpy(nome, "");
        //system("pause");
        i++;
    }


}

}

// Atualiza o vetor na memória na hora de inserir algum registro
void fetch_Vector(FILE *arqKEYS, FILE *arqSEC_NAMES, char *id, char *disc, char *nome, int offset, CHAVE *vector_keys, INDICE_NOMES *vector_nomes){
    CHAVE insere_vetor;
    INDICE_NOMES insere_nome;
    int num_keys = 0;
    int num_nomes = 0;
    int num_ids;
    int num_inseridos = 0;
    int  i = 0;
    int achou = 0;

    strcpy(insere_vetor.id_aluno, id);
    strcpy(insere_vetor.disc, disc);
    strcpy(insere_nome.nome, nome);
    insere_vetor.offset = offset;

    fseek(arqKEYS, sizeof(int), SEEK_SET);
    fseek(arqSEC_NAMES, 0, SEEK_SET);
    fread(&num_keys, sizeof(int), 1, arqKEYS);
    fread(&num_nomes, sizeof(int), 1, arqSEC_NAMES);
    fseek(arqSEC_NAMES, sizeof(int), SEEK_CUR);
    fread(&num_ids, sizeof(int), 1, arqSEC_NAMES);
    fseek(arqKEYS, sizeof(int), SEEK_SET);
    fseek(arqSEC_NAMES, 0, SEEK_SET);
    vector_keys[num_keys] = insere_vetor;
    vector_keys[num_keys].id_aluno[7] = '\0';
    vector_keys[num_keys].disc[3] = '\0';

    
    while(i <= num_nomes && achou != 1){
        
        if(strcmp(vector_nomes[i].nome, insere_nome.nome) == 0){
            achou = 1;
            
            vector_nomes[i].indices[vector_nomes[i].tam_vector] = vector_keys[num_ids];
            vector_nomes[i].indices[vector_nomes[i].tam_vector].id_aluno[7] = '\0';
            vector_nomes[i].indices[vector_nomes[i].tam_vector].disc[3] = '\0';
            vector_nomes[i].tam_vector = vector_nomes[i].tam_vector + 1;
            
            num_ids  = num_ids + 1;
            fseek(arqSEC_NAMES, 2*sizeof(int), SEEK_SET);
            fwrite(&num_ids, sizeof(int), 1, arqSEC_NAMES);
        }

        i++;
    }
    i = 0;
   
    if(achou != 1){
        strcpy(vector_nomes[num_nomes].nome, insere_nome.nome);
        vector_nomes[num_nomes].indices[vector_nomes[num_nomes].tam_vector] = vector_keys[num_ids];
        vector_nomes[num_nomes].indices[vector_nomes[num_nomes].tam_vector].id_aluno[7] = '\0';
        vector_nomes[num_nomes].indices[vector_nomes[num_nomes].tam_vector].disc[3] = '\0';
        vector_nomes[num_nomes].tam_vector = vector_nomes[num_nomes].tam_vector + 1;
        num_ids = num_ids + 1;
        num_nomes = num_nomes + 1;
        fseek(arqSEC_NAMES, 0, SEEK_SET);
        fwrite(&num_nomes, sizeof(int), 1, arqSEC_NAMES);        
        fseek(arqSEC_NAMES, 2*sizeof(int), SEEK_SET);
        fwrite(&num_ids, sizeof(int), 1, arqSEC_NAMES);
        
    }

  
    fseek(arqSEC_NAMES, sizeof(int), SEEK_SET);
    fread(&num_inseridos, sizeof(int), 1, arqSEC_NAMES);
    num_inseridos = num_inseridos + 1;
    fseek(arqSEC_NAMES, sizeof(int), SEEK_SET);
    fwrite(&num_inseridos, sizeof(int), 1, arqSEC_NAMES);
    num_keys = num_keys + 1;
    fwrite(&num_keys, sizeof(int), 1, arqKEYS);
    
    fseek(arqKEYS, 0, SEEK_SET);



}







void Insere_Registro_caso_2(FILE *arqIN, FILE *arqREG, FILE *arqKEYS, FILE *arqSEC_NAMES, CHAVE *vector_keys, INDICE_NOMES *vector_nomes){

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


    fetch_Vector(arqKEYS, arqSEC_NAMES , registro.id_aluno, registro.sigla_disc, registro.nome_aluno, offset, vector_keys, vector_nomes);
    fseek(arqSEC_NAMES, 0, SEEK_SET);

    



}



}





int main(void){

FILE *arqIN;
FILE *arqREG;
FILE *arqKEYS;
FILE *arqBUSCP;
FILE *arqBUSCS;
FILE *arqSEC_NAMES;
FILE *arqSEC_KEYS;

int tam = 200;
CHAVE vector_keys[tam];
INDICE_NOMES vector_nomes[tam];
int i;

for(i = 0; i< tam ; i++){
    vector_nomes[i].tam_vector = 0;
}
i=0;
int nao_atualizado = 0;
int atualizado = 1;
int num_keys = 0;






    
int sair=0;




while(sair != 5){

system("cls");


printf("                  ---- ---- MENU ---- ----           \n\n");
printf("1  -  Carregar arquivos \n");
printf("2  -  Inserir um Registro \n");
printf("3  -  Busca Primaria \n");
printf("4  -  Busca secundaria \n");
printf("5  -  Sair              \n\n");

scanf("%d", &sair);







switch (sair){

case 1:


    if((arqBUSCP = fopen("busca_p.bin", "r+b")) == NULL){
        system("cls");
        printf("\nErro na leitura do arquivo de busca primaria.");
        exit(0);
        Sleep(1300);
        system("cls");
    }else{
        system("cls");
        printf("\n Arquivo de busca de chaves primario aberto.\n");
        Sleep(1300);
        system("cls");
    }

    if((arqIN = fopen("insere.bin", "r+b")) == NULL){
            printf("\nErro na leitura do arquivo de insercao.");
            exit(0);
    }

    if((arqREG = fopen("registros.bin", "r+b")) == NULL){
        int initPegaRegistro = 0; // Onde parei de ler no arquivo de inserÃ§Ã£o
        system("cls");
        printf("\n Arquivo de saida nao existente, criando um novo...\n ");
        Sleep(1300);
        if((arqREG = fopen("registros.bin", "w+b")) == NULL){
            printf("\nFalha na criacao do arquivo.");
        }else{
            fwrite(&initPegaRegistro, sizeof(int), 1, arqREG);
        }
    }else{
        system("cls");
        printf("\n Arquivo de registros carregado com sucesso !\n ");
        Sleep(1300);
    }

    if((arqKEYS = fopen("primary_keys.bin", "r+b")) == NULL){
        system("cls");
        printf("\n Arquivo de chaves primarias nao existente, criando um novo...\n ");
        Sleep(1300);
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
        Sleep(1300);

    }
    // arqSEC_NAMES : HEADERS
    // HEADER 1: Número de nomes inseridos no arquivo
    // HEADER 2: Número de chaves nome - id - disc inseridas
    // HEADER 3: Número de id's- disc inseridos
    // arqSEC_NAMES: CORPO
    // TAM_DO_VETOR(DE ID's E DISC's para um nome no outro arquivo) seguido de NOME DO USUÁRIO
    if((arqSEC_NAMES = fopen("secondary_names.bin", "r+b")) == NULL){
        system("cls");
        printf("\n Arquivo de nomes secundario nao existente, criando um novo...\n ");
        Sleep(1300);
        if((arqSEC_NAMES = fopen("secondary_names.bin", "w+b")) == NULL){
            printf("\nFalha na criacao do arquivo de nomes secundarios.");
        }else{
            fwrite(&num_keys, sizeof(int), 1, arqSEC_NAMES);
            fwrite(&num_keys, sizeof(int), 1, arqSEC_NAMES);
            fwrite(&num_keys, sizeof(int), 1, arqSEC_NAMES);
        }
    }else{
        fseek(arqSEC_NAMES, 0, SEEK_SET);
        printf("\n Arquivo de nomes secundario carregado com sucesso !\n ");
        Sleep(1300);
    }
    if((arqSEC_KEYS = fopen("secondary_keys.bin", "r+b")) == NULL){
        system("cls");
        printf("\n Arquivo de chaves secundario nao existente, criando um novo...\n ");
        Sleep(1300);
        if((arqSEC_KEYS = fopen("secondary_keys.bin", "w+b")) == NULL){
            printf("\nFalha na criacao do arquivo de chaves secundarias.");
        }else{
            
        }
    }else{
        fseek(arqSEC_KEYS, 0, SEEK_SET);
        get_vector_secondary(arqSEC_NAMES, arqSEC_KEYS, vector_nomes);
        printf("\n Arquivo de chaves secundarias carregado com sucesso !\n ");
        Sleep(1300);
    }
    if((arqBUSCS = fopen("busca_s.bin", "r+b")) == NULL){
        system("cls");
        printf("\n Arquivo de busca secundario nao foi possivel de ser aberto, veja se o mesmo existe...\n ");
        Sleep(1300);
    
    }else{
        system("cls");
        printf("\n Arquivo de busca de chaves secundario aberto.\n");
        Sleep(1300);
    }
    break;
case 2: 
    Insere_Registro_caso_2(arqIN, arqREG, arqKEYS, arqSEC_NAMES, vector_keys, vector_nomes);
    Sleep(1000);


    break;

case 3:
    busca_chave_valor_primaria(arqREG, arqKEYS, arqBUSCP, vector_keys);
    system("pause");
    break;


case 4:
    busca_chave_valor_secundaria(arqSEC_NAMES, arqSEC_KEYS, arqREG, arqBUSCS, arqKEYS, vector_keys);
    system("pause");
    break;


case 5: 
    ordena_vetor_insere(arqKEYS, vector_keys, arqSEC_NAMES, arqSEC_KEYS, vector_nomes);
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












