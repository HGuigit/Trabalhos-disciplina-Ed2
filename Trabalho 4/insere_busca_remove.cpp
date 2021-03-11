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
    } vet[3] = { {"0000001", "333", "Nome-1", "Disc-333", 5.4, 79.7},
                 {"0000001", "346", "Nome-1", "Disc-346", 8.2, 80.4},
				 {"0000001", "346", "Nome-1", "Disc-346", 8.2, 80.4},//chave duplicada
			   };
       
    fd = fopen("insere.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct busca {
        char id_aluno[8];
        char sigla_disc[4];
    } vet_b[3] = { {"0000001","333"},
                   {"0000001","346"},
                   {"0000001","888"},//chave inexistente
				 };
    
    fd = fopen("busca.bin", "w+b");
    fwrite(vet_b, sizeof(vet_b), 1, fd);
    fclose(fd);    
    
    //////////////////////////////
    struct remove {
        char id_aluno[8];
        char sigla_disc[4];
    } vet_r[3] = { {"0000001","333"},
                   {"0000001","346"},
                   {"0000001","888"}//chave inexistente
				 };
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

