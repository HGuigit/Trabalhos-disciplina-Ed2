#include<stdio.h>
#include<conio.h>
#include<string.h>

int main() {
    FILE *fd;
    
    //////////////////////////////   
    struct hist {
        char id_aluno[8];
        char sigla_disc[4];
        char nome_aluno[50];
        char nome_disc[50];
        float media;
        float freq;
    } vet[6] = {{"0000001", "333", "Nome-1", "Disc-333", 5.4, 79.7},
                {"0000001", "222", "Nome-1", "Disc-222", 8.2, 80.4},
                {"0000002", "444", "Nome-2", "Disc-444", 6.8, 91.3},            
                {"0000003", "666", "Nome-3", "Disc-666", 8.3, 77.7},                
	            {"0000001", "111", "Nome-1", "Disc-111", 7.5, 72.3},            
                {"0000002", "555", "Nome-2", "Disc-555", 6.3, 72.7}};
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct busca {
        char id_aluno[8];
        char sigla_disc[4];
    } vet_b[3] = {{"0000001","222"},
                  {"0000001","777"},
                  {"0000001","222"}};
    
    fd = fopen("busca.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);    
    
    //////////////////////////////
    struct consulta {
        char id_aluno[8];
        char sigla_disc[4];
    } vet_c[2] = {{"0000001","222"},
                  {"0000001","777"}};
    
    fd = fopen("consulta.bin", "w+b");
    fwrite(vet_c, sizeof(vet_c), 1, fd);
    fclose(fd); 
}

