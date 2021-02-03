#include<stdio.h>
#include<conio.h>

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
    } vet[8] = {{"0000001", "111", "Joao da Silva", "Disc-111", 7.5, 72.3},
                {"0000001", "222", "Joao da Silva", "Disc-222", 8.2, 80.4},
                {"0000001", "333", "Joao da Silva", "Disc-333", 5.4, 79.7},                
                {"0000002", "444", "Paulo Souza", "Disc-444", 6.8, 91.3},
            
                {"0000003", "555", "Pedro", "Disc-555", 6.3, 72.7},
                {"0000001", "666", "Joao da Silva", "Disc-666", 8.3, 77.7},
                {"0000003", "777", "Pedro", "Disc-777", 9.7, 89.7},
                
                {"0000005", "888", "Lucas Silva e Silva", "Disc-888-888", 9.5, 92.7}};
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
    struct busca_p {
        char id_aluno[8];
        char sigla_disc[4];
    } vet_bp[4] = {{"0000001","333"},
                  {"0000002","444"},
                  
                  {"0000003","777"},
                  {"0000001","111"}};
    
    fd = fopen("busca_p.bin", "w+b");
    fwrite(vet_bp, sizeof(vet_bp), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	const int quantidade = 5;
    char nomes[quantidade][50] = {"Joao da Silva", "Paulo Souza", "Joao da Silva", "Pedro"};
       
    fd = fopen("busca_s.bin", "w+b");
    for (int i=0; i<quantidade; i++)
       fwrite(nomes[i], sizeof(char), 50, fd);
    fclose(fd);
    
    /*char buffer[50];
    fd = fopen("busca_s.bin", "r+b");
    for (int i=0; i<quantidade; i++)
       {
         fread(buffer, sizeof(buffer), 1, fd);
         printf("\n%s",buffer);
       }
    fclose(fd);
    getch();*/
}

