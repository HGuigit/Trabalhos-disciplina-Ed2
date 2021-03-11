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

FILE *abrirB()//abre arquivo para busca
{
    FILE *arq=fopen("busca.bin", "rb");
	return arq;
}

FILE* abrirC()//abre arquivo para busca
{
    FILE *arq=fopen("consulta.bin", "rb");
    return arq;
}

FILE* criarArqMenores(){

FILE *arq = fopen("menores.bin", "wb+");
return arq;

}
FILE* criarArqMaiores(){

FILE *arq = fopen("maiores.bin", "wb+");
return arq;

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


int listaElementosInOrdem(int posi,FILE *arqARVB, FILE *arqREG)
{
    nodo test;
    int qnt,counter=0;
    inserir printa;
    fseek(arqARVB,posi,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    while(counter<qnt)
    {
        if(test.no[counter].esq!=-1)
        {
            listaElementosInOrdem(test.no[counter].esq,arqARVB,arqREG);
            
        }
        fseek(arqREG, test.no[counter].offset_arqPrincipal, SEEK_SET);
        fread(&printa, sizeof(inserir), 1, arqREG);
        printf("\n\nID: %s\nSigla Disc: %s\nNome: %s\nNome disciplina: %s\nMedia: %2.f\nFrequencia: %2.f\n\n", printa.id_aluno, printa.sigla_disc, printa.nome_aluno, printa.nome_disc, printa.media, printa.freq);
        counter++;
    }
    if(test.no[counter-1].dir!=-1)
    {
        listaElementosInOrdem(test.no[counter-1].dir,arqARVB,arqREG);

    }
        
      

}
void matchsort(FILE *arqMENORES, FILE *arqMAIORES)
{
    FILE *match=fopen("match.bin","wb");
    busca ele1,ele2;
    int percorrer=0;
    rewind(arqMENORES);
    rewind(arqMAIORES);
    percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
    percorrer=fread(&ele2,sizeof(busca), 1, arqMAIORES);
    while(percorrer!=0)
    {
        
        if(strcmp(ele1.id_aluno, ele2.id_aluno) == 0 && strcmp(ele1.sigla_disc, ele2.sigla_disc) == 0)
        {
            fwrite(&ele1,sizeof(busca),1,match);
            percorrer=fread(&ele2,sizeof(busca), 1, arqMAIORES);
        } 
        else if(strcmp(ele1.id_aluno, ele2.id_aluno) > 0)
        {
            percorrer=fread(&ele2,sizeof(busca), 1, arqMAIORES);
        }
        else if(strcmp(ele1.id_aluno, ele2.id_aluno) < 0)
        {
            percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
        }
        else if(strcmp(ele1.id_aluno, ele2.id_aluno) == 0){
            if(strcmp(ele1.sigla_disc, ele2.sigla_disc) > 0)
            {
                percorrer=fread(&ele2,sizeof(busca), 1, arqMAIORES);
            }
            else if(strcmp(ele1.sigla_disc, ele2.sigla_disc) < 0)
            {
                percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
            }
        }
    }
    fclose(match);
}

void mergesort(FILE *arqMENORES, FILE *arqMAIORES)
{
    FILE *merge=fopen("merge.bin","wb");
    busca ele1,ele2;
    int percorrer=0,percorrer2=0;
    rewind(arqMENORES);
    rewind(arqMAIORES);
    percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
    percorrer2=fread(&ele2,sizeof(busca), 1, arqMAIORES);
    while(percorrer!=0 && percorrer2!=0)
    {
        
        if(strcmp(ele1.id_aluno, ele2.id_aluno) == 0 && strcmp(ele1.sigla_disc, ele2.sigla_disc) == 0)
        {
            fwrite(&ele1,sizeof(busca),1,merge);
            percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
            percorrer2=fread(&ele2,sizeof(busca), 1, arqMAIORES);
        } 
        else if(strcmp(ele1.id_aluno, ele2.id_aluno) > 0)
        {
        	fwrite(&ele2,sizeof(busca),1,merge);
            percorrer2=fread(&ele2,sizeof(busca), 1, arqMAIORES);
        }
        else if(strcmp(ele1.id_aluno, ele2.id_aluno) < 0)
        {
        	fwrite(&ele1,sizeof(busca),1,merge);
            percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
        }
        else if(strcmp(ele1.id_aluno, ele2.id_aluno) == 0){
            if(strcmp(ele1.sigla_disc, ele2.sigla_disc) > 0)
            {
            	fwrite(&ele2,sizeof(busca),1,merge);
                percorrer2=fread(&ele2,sizeof(busca), 1, arqMAIORES);
            }
            else if(strcmp(ele1.sigla_disc, ele2.sigla_disc) < 0)
            {
            	fwrite(&ele1,sizeof(busca),1,merge);
                percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
            }
        }
    }
    if(percorrer!=percorrer2)
    {
    	if(percorrer!=0)
    	{
    		while(percorrer!=0)
    		{
    			fwrite(&ele1,sizeof(busca),1,merge);
            	percorrer=fread(&ele1,sizeof(busca), 1, arqMENORES);
			}
		}
		else if(percorrer2!=0)
		{
			while(percorrer2!=0)
    		{
    			fwrite(&ele2,sizeof(busca),1,merge);
            	percorrer2=fread(&ele2,sizeof(busca), 1, arqMAIORES);
			}
		}
	}
    fclose(merge);
}

int listaElementosMenoresIguaisX(int posi,FILE *arqARVB, busca procura, FILE *arqMENORES, int flag)
{
    nodo test;
    int qnt,counter=0;
    inserir printa;
    fseek(arqARVB,posi,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    
    while(counter<qnt)
    {
        if(test.no[counter].esq!=-1)
        {
            flag=listaElementosMenoresIguaisX(test.no[counter].esq,arqARVB,procura,arqMENORES,flag);
            
        }
        if(strcmp(test.no[counter].id_aluno, procura.id_aluno) == 0)
		{
			if(strcmp(test.no[counter].sigla_disc, procura.sigla_disc) > 0)
            	{flag = 1;}
        }
        else if(strcmp(test.no[counter].id_aluno, procura.id_aluno) > 0)
        {
        	flag = 1;
		}
        if(flag == 0){
        fseek(arqMENORES, 0, SEEK_END);   
        fwrite(&test.no[counter].id_aluno, sizeof(test.no[counter].id_aluno), 1, arqMENORES);
        fwrite(&test.no[counter].sigla_disc, sizeof(test.no[counter].sigla_disc), 1, arqMENORES);
        }
        counter++;
    }
    if(test.no[counter-1].dir!=-1)
    {
        flag=listaElementosMenoresIguaisX(test.no[counter-1].dir,arqARVB,procura,arqMENORES,flag);

    }
        
    return flag;

}

int listaElementosMaioresIguaisX(int posi,FILE *arqARVB, busca procura, FILE *arqMAIORES, int flag)
{
    nodo test;
    int qnt,counter=0;
    inserir printa;
    fseek(arqARVB,posi,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    
    while(counter<qnt)
    {
    	if(test.no[counter].esq!=-1)
        {
            flag=listaElementosMaioresIguaisX(test.no[counter].esq,arqARVB,procura,arqMAIORES,flag);
            
        }
        if(strcmp(test.no[counter].id_aluno, procura.id_aluno) == 0)
		{
			if(strcmp(test.no[counter].sigla_disc, procura.sigla_disc) >= 0)
            	{flag = 0;}
        }
        else if(strcmp(test.no[counter].id_aluno, procura.id_aluno) > 0)
        {
        	flag = 0;
		}
        if(flag == 0){
        fseek(arqMAIORES, 0, SEEK_END);   
        fwrite(&test.no[counter].id_aluno, sizeof(test.no[counter].id_aluno), 1, arqMAIORES);
        fwrite(&test.no[counter].sigla_disc, sizeof(test.no[counter].sigla_disc), 1, arqMAIORES);
        }
        counter++;
    }
    if(test.no[counter-1].dir!=-1)
    {
        flag=listaElementosMaioresIguaisX(test.no[counter-1].dir,arqARVB,procura,arqMAIORES,flag);

    }
        
    return flag;
}

int buscarID(busca aux, int posi,FILE *arqARVB,FILE *arqREG)
{
    nodo test;
    inserir printa;
    int qnt,counter=0;
    fseek(arqARVB,posi,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    while(counter<qnt)
    {
        if((strcmp(test.no[counter].id_aluno,aux.id_aluno) == 0)&&(strcmp(test.no[counter].sigla_disc,aux.sigla_disc) == 0))
        {
            posi=posi-8;
            posi=posi/100;
            fseek(arqREG,test.no[counter].offset_arqPrincipal,SEEK_SET);
            fread(&printa,sizeof(inserir),1,arqREG);
            printf("\n\nID: %s\nSigla Disc: %s\nNome: %s\nNome disciplina: %s\nMedia: %2.f\nFrequencia: %2.f\n", printa.id_aluno, printa.sigla_disc, printa.nome_aluno, printa.nome_disc, printa.media, printa.freq);
            printf("Pagina: %d\nPosicao: %d\n\n",posi,counter);
            return 1;  
        }
        if((strcmp(aux.id_aluno,test.no[counter].id_aluno) > 0))
        {
            counter++;    
        }
        else{
        	if(strcmp(test.no[counter].id_aluno,aux.id_aluno) == 0)
        	{
	        	if(strcmp(aux.sigla_disc,test.no[counter].sigla_disc) > 0)
	        	{
	        		counter++;	
				}
				else{
					if(test.no[counter].esq==-1)
			        {
			            printf("Nao achado\n");
			            return 0;
			        }
            		return(buscarID(aux, test.no[counter].esq,arqARVB,arqREG));
				}
			}
			else{
					if(test.no[counter].esq==-1)
			        {
			            printf("Nao achado\n");
			            return 0;
			        }
            		return(buscarID(aux, test.no[counter].esq,arqARVB,arqREG));
				}
		}
        
    }
    if(test.no[counter-1].dir==-1)
    {
        
        printf("Nao achado\n");
        return 0;
    }
    return (buscarID(aux, test.no[counter-1].dir,arqARVB,arqREG));
}


int buscar(inserir add, int posi,FILE *arqARVB)
{
    nodo test;
    int qnt,counter=0;
    fseek(arqARVB,posi,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    while(counter<qnt)
    {
        if((strcmp(test.no[counter].id_aluno,add.id_aluno) == 0)&&(strcmp(test.no[counter].sigla_disc,add.sigla_disc) == 0))
        {
            printf("\nJá existe na árvore.\n");
            return 1;  
        }
        if((strcmp(add.id_aluno,test.no[counter].id_aluno) > 0))
        {
            counter++;    
        }
        else{
        	if(strcmp(test.no[counter].id_aluno,add.id_aluno) == 0)
        	{
	        	if(strcmp(add.sigla_disc,test.no[counter].sigla_disc) > 0)
	        	{
	        		counter++;	
				}
				else{
					if(test.no[counter].esq==-1)
			        {
			            printf("Nao achado\n");
			            return 0;
			        }
            		return(buscar(add, test.no[counter].esq,arqARVB));
				}
			}
			else{
					if(test.no[counter].esq==-1)
			        {
			            printf("Nao achado\n");
			            return 0;
			        }
            		return(buscar(add, test.no[counter].esq,arqARVB));
				}
		}
        
    }
    if(test.no[counter-1].dir==-1)
    {
        
        printf("Nao achado\n");
        return 0;
    }
    return (buscar(add, test.no[counter-1].dir,arqARVB));
}



nodo OrganizaNoh(int qnt,nodo test,inserir add,int offset)
{
    int i, j;
    casa_nodo insere,aux;
    strcpy(insere.id_aluno, add.id_aluno);
    strcpy(insere.sigla_disc, add.sigla_disc);
    insere.dir = -1;
    insere.esq = -1;
    insere.offset_arqPrincipal = offset;
    test.no[qnt] = insere;
    qnt++;
    //bubblesort
    for(i=0; i<qnt ; i++)
    {
        for(j=0; j<qnt-1 ; j++)
        {
            if((strcmp(test.no[j].id_aluno,test.no[j+1].id_aluno) > 0))
            {
                aux=test.no[j];
                test.no[j]=test.no[j+1];
                test.no[j+1]=aux;
            }else if((strcmp(test.no[j].id_aluno,test.no[j+1].id_aluno) == 0)){
                if((strcmp(test.no[j].sigla_disc,test.no[j+1].sigla_disc) > 0)){
                    aux=test.no[j];
                    test.no[j]=test.no[j+1];
                    test.no[j+1]=aux;
                }
            }
        }

    }

    return (test);

}

nodo OrganizaNohVelho(int qnt,nodo test)
{
    int i, j;
    casa_nodo insere,aux;
    //bubblesort
    for(i=0; i<qnt ; i++)
    {
        for(j=0; j<qnt-1 ; j++)
        {
            if((strcmp(test.no[j].id_aluno,test.no[j+1].id_aluno) > 0))
            {
                aux=test.no[j];
                test.no[j]=test.no[j+1];
                test.no[j+1]=aux;
            }else if((strcmp(test.no[j].id_aluno,test.no[j+1].id_aluno) == 0)){
                if((strcmp(test.no[j].sigla_disc,test.no[j+1].sigla_disc) > 0)){
                    aux=test.no[j];
                    test.no[j]=test.no[j+1];
                    test.no[j+1]=aux;
                }
            }
        }

    }

    return (test);

}


void nova_raiz(casa_nodo noh,FILE *arqARVB)
{
    nodo raiz;
    raiz.no[0]=noh;
    int posi,tam=1;
    fseek(arqARVB,0,SEEK_END);
    posi=ftell(arqARVB);
    fwrite(&raiz,sizeof(nodo),1,arqARVB);//poe no final a raiz
    fwrite(&tam,sizeof(int),1,arqARVB);
    fseek(arqARVB,0,SEEK_SET);
    fwrite(&posi,sizeof(int),1,arqARVB);//muda o header pra posicao nova da raiz
}


casa_nodo Split(nodo noh,int posi, FILE *arqARVB)
{
    casa_nodo aux;
    nodo maior, menor;
    int counter=0;
    menor.no[0]=noh.no[0];//divide entre menor e maior o noh
    aux=noh.no[1];
    maior.no[0]=noh.no[2];
    maior.no[1]=noh.no[3];
    printf("\nDivisao de Noh!\n");
    printf("\nChave %s  %s  promovida com sucesso!", aux.id_aluno, aux.sigla_disc);
    fseek(arqARVB,posi,SEEK_SET);
    aux.esq=ftell(arqARVB);
    fwrite(&menor,sizeof(nodo),1,arqARVB);//escreve o noh menor na mesma posicao
    counter=1;
    fwrite(&counter,sizeof(int),1,arqARVB);
    fseek(arqARVB,0,SEEK_END);
    aux.dir=ftell(arqARVB);
    fwrite(&maior,sizeof(nodo),1,arqARVB);
    counter=2;
    fwrite(&counter,sizeof(int),1,arqARVB);
    return aux;//vai voltar recursivamente


}

nodo iguala(nodo noh,casa_nodo aux,int qnt)
{
	int counter=0;
	while(counter<4)
	{
		if((strcmp(noh.no[counter].id_aluno,aux.id_aluno)==0)&&(strcmp(noh.no[counter].sigla_disc,aux.sigla_disc)==0))
		{
			if(counter!=0)
			{
				if(counter<(qnt-1))
				{
					noh.no[counter-1].dir=noh.no[counter].esq;
					noh.no[counter+1].esq=noh.no[counter].dir;
					return noh;
				}
				else{
					noh.no[counter-1].dir=noh.no[counter].esq;
					return noh;
				}
			}
			else{
				noh.no[counter+1].esq=noh.no[counter].dir;
				return noh;
			}
		}
		counter++;
	}
}

casa_nodo insereNaArvore(int offset,int posiAnterior, int posi, inserir add,FILE *arqARVB)
{
    nodo test,nova;
    casa_nodo aux;
    int qnt,counter=0,flag=0;
    fseek(arqARVB,posi,SEEK_SET);
    fread(&test,sizeof(nodo),1,arqARVB);
    fread(&qnt,sizeof(int),1,arqARVB);
    while(counter<qnt)
    {
        // percorre página até achar um lugar onde a chave a ser adicionada é menor que a atual
        if((strcmp(add.id_aluno,test.no[counter].id_aluno) > 0))
        {
            counter++;    
        }
        else{
            // Caso os id's sejam iguais
                if(strcmp(test.no[counter].id_aluno,add.id_aluno) == 0)
                {
                    // Lógica de comparação das disciplinas em caso de id igual
                    if(strcmp(add.sigla_disc,test.no[counter].sigla_disc) > 0)
                    {
                        counter++;
                    }
                    else{
                        //Ponteiro a esquerda é -1 (Nó folha)
                        if(test.no[counter].esq==-1)
                        {
                            // Página Cheia, split e promoção efetuado
                            if(qnt==3)
                            {
                                if(posi==posiAnterior)
                                    {
                                        nova=OrganizaNoh(qnt,test,add,offset);
                                        aux=(Split(nova,posi,arqARVB));
                                        nova_raiz(aux,arqARVB);
                                        fseek(arqARVB,4,SEEK_SET);
                                        flag=0;
                                        fwrite(&flag,sizeof(int),1,arqARVB);
                                        return aux;
                                    }
                                    else{
                                        flag=1;
                                        fseek(arqARVB,4,SEEK_SET);
                                        fwrite(&flag,sizeof(int),1,arqARVB);
                                        nova=OrganizaNoh(qnt,test,add,offset);
                                        return(Split(nova,posi,arqARVB));
                                    }

                            }
                            // Página não cheia, insere na página e organiza ela novamente
                            else{
                                nova=OrganizaNoh(qnt,test,add,offset);
                                fseek(arqARVB,posi,SEEK_SET);
                                fwrite(&nova,sizeof(nodo),1,arqARVB);
                                qnt++;
                                fwrite(&qnt, sizeof(int) , 1, arqARVB);
                                return aux;
                                
                            }
                        }
                        //Não é nó folha
                        else{
                            //Percorre árvore recursivamente para a esquerda
                            aux=insereNaArvore(offset, posi, test.no[counter].esq,add,arqARVB);
                            fseek(arqARVB,4,SEEK_SET);
                            fread(&flag,sizeof(int),1,arqARVB);
                            //No retorno das chamadas checa se a flag está setada em 1, se a flag estiver setada em 1 significa que algum elemento foi promovido, logo deve tentar inserir na página atual
                            if(flag==1)
                            {
                                //Lógica para se o nó atual estiver cheio, a promoção deve ser inserida, a organização efetuada e o split promoção deve ocorrer novamente
                                if(qnt==3)
                                {
                                    test.no[3]=aux;
                                    //Checagem se o nó atual é na realidade uma raiz
                                    if(posi==posiAnterior)
                                    {
                                        nova=OrganizaNohVelho((qnt+1),test);
                                        aux=(Split(nova,posi,arqARVB));
                                        nova_raiz(aux,arqARVB);
                                        fseek(arqARVB,4,SEEK_SET);
                                        flag=0;
                                        fwrite(&flag,sizeof(int),1,arqARVB);
                                        return aux;
                                    }
                                    else{
                                        nova=OrganizaNohVelho((qnt+1),test);
                                        return(Split(nova,posi,arqARVB));
                                    }
                                }
                                // página não está cheia, organiza e insere, seta flag para 0.
                                else{
                                    test.no[qnt]=aux;
                                    nova=OrganizaNohVelho((qnt+1),test);
                                    nova=iguala(nova,aux,(qnt+1));
                                    fseek(arqARVB,posi,SEEK_SET);
                                    fwrite(&nova,sizeof(nodo),1,arqARVB);
                                    qnt++;
                                    fwrite(&qnt, sizeof(int) , 1, arqARVB);
                                    flag=0;
                                    fseek(arqARVB,4,SEEK_SET);
                                    fwrite(&flag,sizeof(int),1,arqARVB);
                                    return aux;
                                }

                            }
                            else{
                            	return aux;
							}
                        }
                    }
                }
                else{
                    if(test.no[counter].esq==-1)
                    {
                            if(qnt==3)
                            {
                                if(posi==posiAnterior)
                                    {
                                        nova=OrganizaNoh(qnt,test,add,offset);
                                        aux=(Split(nova,posi,arqARVB));
                                        nova_raiz(aux,arqARVB);
                                        fseek(arqARVB,4,SEEK_SET);
                                        flag=0;
                                        fwrite(&flag,sizeof(int),1,arqARVB);
                                        return aux;
                                    }
                                    else{
                                        flag=1;
                                        fseek(arqARVB,4,SEEK_SET);
                                        fwrite(&flag,sizeof(int),1,arqARVB);
                                        nova=OrganizaNoh(qnt,test,add,offset);
                                        return(Split(nova,posi,arqARVB));
                                    }
                            }else{
                                nova=OrganizaNoh(qnt,test,add,offset);
                                fseek(arqARVB,posi,SEEK_SET);
                                fwrite(&nova,sizeof(nodo),1,arqARVB);
                                qnt++;
                                fwrite(&qnt, sizeof(int) , 1, arqARVB);
                                return aux;
                            }
                           
                    }
                    else
                    {
                        aux=insereNaArvore(offset, posi, test.no[counter].esq,add,arqARVB);
                        fseek(arqARVB,4,SEEK_SET);
                        fread(&flag,sizeof(int),1,arqARVB);
                        if(flag==1)
                        {
                            if(qnt==3)
                            {
                                test.no[3]=aux;
                                if(posi==posiAnterior)
                                    {
                                        nova=OrganizaNohVelho((qnt+1),test);
                                        aux=(Split(nova,posi,arqARVB));
                                        nova_raiz(aux,arqARVB);
                                        fseek(arqARVB,4,SEEK_SET);
                                        flag=0;
                                        fwrite(&flag,sizeof(int),1,arqARVB);
                                        return aux;
                                    }
                                    else{
                                        nova=OrganizaNohVelho(4,test);
                                        return(Split(nova,posi,arqARVB));
                                    }
                            }else{
                                test.no[qnt]=aux;
                                nova=OrganizaNohVelho((qnt+1),test);
                                nova=iguala(nova,aux,(qnt+1));
                                fseek(arqARVB,posi,SEEK_SET);
                                fwrite(&nova,sizeof(nodo),1,arqARVB);
                                qnt++;
                                fwrite(&qnt, sizeof(int) , 1, arqARVB);
                                flag=0;
                                fseek(arqARVB,4,SEEK_SET);
                                fwrite(&flag,sizeof(int),1,arqARVB);
                                return aux;
                            }
                        }
                        else{
                        	return aux;
						}
                    }
                }
           
        }
    } 
    if(test.no[counter-1].dir==-1)
    {
        if(qnt==3)
        {
            if(posi==posiAnterior)
                {
                    nova=OrganizaNoh(qnt,test,add,offset);
                    aux=(Split(nova,posi,arqARVB));
                    nova_raiz(aux,arqARVB);
                    fseek(arqARVB,4,SEEK_SET);
                    flag=0;
                    fwrite(&flag,sizeof(int),1,arqARVB);
                    return aux;
                }
                else{
                    flag=1;
                    fseek(arqARVB,4,SEEK_SET);
                    fwrite(&flag,sizeof(int),1,arqARVB);
                    nova=OrganizaNoh(qnt,test,add,offset);
                    return(Split(nova,posi,arqARVB));
                }
        }else{
            nova=OrganizaNoh(qnt,test,add,offset);
            fseek(arqARVB,posi,SEEK_SET);
            fwrite(&nova,sizeof(nodo),1,arqARVB);
            qnt++;
            fwrite(&qnt, sizeof(int) , 1, arqARVB);
            return aux;
        }
       
	}
	else
	{
	    aux=insereNaArvore(offset, posi, test.no[counter-1].dir,add,arqARVB);
	    fseek(arqARVB,4,SEEK_SET);
	    fread(&flag,sizeof(int),1,arqARVB);
	    if(flag==1)
	    {
	        if(qnt==3)
	        {
	            test.no[3]=aux;
	            if(posi==posiAnterior)
	                {
	                    nova=OrganizaNohVelho((qnt+1),test);
	                    aux=(Split(nova,posi,arqARVB));
	                    nova_raiz(aux,arqARVB);
	                    fseek(arqARVB,4,SEEK_SET);
	                    flag=0;
	                    fwrite(&flag,sizeof(int),1,arqARVB);
	                    return aux;
	                }
	                else{
	                    nova=OrganizaNohVelho((qnt+1),test);
	                    return(Split(nova,posi,arqARVB));
	                }
	        }else{
	            test.no[qnt]=aux;
	            nova=OrganizaNohVelho((qnt+1),test);
	            nova=iguala(nova,aux,(qnt+1));
	            fseek(arqARVB,posi,SEEK_SET);
	            fwrite(&nova,sizeof(nodo),1,arqARVB);
	            qnt++;
	            fwrite(&qnt, sizeof(int) , 1, arqARVB);
	            flag=0;
	            fseek(arqARVB,4,SEEK_SET);
	            fwrite(&flag,sizeof(int),1,arqARVB);
	            return aux;
	        }
	    }
	    else{
	    	return aux;
		}
	}
    

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
    int raiz=getBtree(arqARVB);
    rewind(arqREG);
    
    if(raiz != -1){

         // Código de inserção de elemento na árvore
        if((buscar(add,raiz, arqARVB)) != 1 )
        {
            fread(&header,sizeof(int),1,arqREG);
            header++;
            fseek(arqREG,0,SEEK_END);
            offset = ftell(arqREG);
            fwrite(&add,sizeof(inserir),1,arqREG);
            rewind(arqREG);
            fwrite(&header,sizeof(int),1,arqREG);
            insereNaArvore(offset, raiz,raiz, add, arqARVB);
            printf("\n%s %s Adicionado\n",add.id_aluno,add.sigla_disc);
        }
        else{
            printf("\nO registro %s %s é duplicado, não pode ser inserido!\n",add.id_aluno,add.sigla_disc);
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
        printf("\n%s %s Adicionado\n",add.id_aluno,add.sigla_disc);        
    
    }
}

int main()
{
    busca input;
    inserir add;
    int opc=0,rep=1,header=0,raiz=0;
    nodo root;

    //Arquivos do programa
    FILE *arqARVB;
    FILE *procura=abrirB();
    FILE *arqREG;
    FILE *arqMENORES;
    FILE *arqMAIORES;
    FILE *arqCONSULTA;

    //Headers arqARVB
    int naoExisteArv = -1;
    int flag = 0;

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
            fwrite(&flag, sizeof(int),1, arqARVB);
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
		printf("Digite a opcao:\nAdicionar-->1\nPrintar alunos-->2\nBusca-->3\nConsulta Casada-->4\nSair-->5\n");
		scanf("%d",&opc);
		switch(opc)
		{
			case 1:	
				add=abrirpAdd(arqREG);
                header=adiciona(add,arqREG, arqARVB);
                system("pause");
                system("cls");
                break;
			case 2:
                fseek(arqARVB,0,SEEK_SET);
                fread(&raiz,sizeof(int),1,arqARVB);
                listaElementosInOrdem(raiz,arqARVB,arqREG);
				break;
			case 3:
                fseek(arqARVB,0,SEEK_SET);
                fread(&raiz,sizeof(int),1,arqARVB);
                fread(&input,sizeof(busca),1,procura);
                buscarID(input, raiz,arqARVB,arqREG);
				break;
            case 4:
                fseek(arqARVB,0,SEEK_SET);
                fread(&raiz,sizeof(int),1,arqARVB);
                rewind(arqARVB);
                arqCONSULTA = abrirC();
                fread(&input, sizeof(busca), 1, arqCONSULTA);
                arqMENORES = criarArqMenores();
                arqMAIORES = criarArqMaiores();
                listaElementosMaioresIguaisX(raiz,arqARVB, input, arqMAIORES, 1);
                rewind(arqARVB);
                fread(&input, sizeof(busca), 1, arqCONSULTA);
                listaElementosMenoresIguaisX(raiz,arqARVB, input, arqMENORES, 0);
                matchsort(arqMENORES, arqMAIORES);
                mergesort(arqMENORES, arqMAIORES);
                fclose(arqMAIORES);
                fclose(arqMENORES);
				break;
			case 5:
				fclose(procura);
				fclose(arqARVB);
				fclose(arqREG);
				return 0;				
		}
	}while(rep==1);
}
