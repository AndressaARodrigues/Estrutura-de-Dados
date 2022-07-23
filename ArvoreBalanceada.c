#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************
*			                    		  		    *
*	  Andressa De Almeida Rodrigues			        *
* 	  Universidade Federal do Pampa (UNIPAMPA)      *
*	  Bacharelado em Ciência da Computação	   	    *
*	  Estrutura de Dados 2 - Árvores Balanceadas    *	         
*								                    *
*****************************************************/

//Melhorias: Busca(Case-sensitive).

typedef struct jogo Jogo;
typedef struct arv Arv;
typedef struct arvore Arvore;

struct jogo{
    char id[61];
    char game[48], series[48] , country[20], banStatus[26], genre[49];
    //banCategory[80], banStatus[25], wikipedia_profile[85], image[159], developer[140], publisher[120], genre[50], homepage[50];
    //Id,Game,Series,Country,Details 934,Ban Category,Ban Status26,Wikipedia Profile85,Image 159,Summary2880,Developer,Publisher,Genre,Homepage98
};

struct arv
{
    Jogo *jogo;
    Arv* esq;
    Arv* dir;
};

struct arvore 
{
    Arv* raiz;
};

/*
    Cria a arvore
*/
Arvore* arv_criavazia(){
    Arvore* novo = (Arvore*)malloc(sizeof(Arvore));
    novo->raiz = NULL;
    return novo;
}

/*
    Verifica se a arvore está vazia
*/
int arv_vazia(Arv* a){
    return (a == NULL);
}

// ---------- ALTURA ------------
/*  
    Retorna o maior entre 2 valores
    a, b -> altura de 2 nós da árvore
*/
int arv_max(int a, int b){
    return (a>b) ? a : b;
}

//Retorna a altura
int arv_altura(Arv* a){
    if(!arv_vazia(a)){
        return 1 + arv_max(arv_altura(a->esq), 
                           arv_altura(a->dir));
    }else
        return 0;
}

int altura(Arvore* r){
    return arv_altura(r->raiz);
}

/* 
    Calcula e retona o fator de balanceamento de um nó 
*/
int fatorB(Arv* r){
    return arv_altura(r->dir) - arv_altura(r->esq);
}

// ---------- ROTAÇÕES ------------
/*
    Rotação simples a esquerda
*/
Arv* rotacaoEsquerda(Arv* p){
    Arv* q;
    Arv* temp;

    q = p->dir;
    temp = q->esq;

    q->esq = p;
    p->dir = temp;

    p = q;
    return p;
}

/*
    Rotação simples a direira
*/
Arv* rotacaoDireita(Arv* p){
    Arv* q; 
    Arv* temp;
    
    q = p->esq;
    temp = q->dir;

    q->dir = p;
    p->esq = temp;
       
    p = q;
    return p;
}

/*
    Rotação dupla direita e esquerda
*/
Arv* rotacaoDirEsq(Arv* p){
    p->dir = rotacaoDireita(p->dir);
    p = rotacaoEsquerda(p);
    return p;
}

/*
    Rotação dupla esquerda e direita
*/
Arv* rotacaoEsqDir(Arv* p){
    p->esq = rotacaoEsquerda(p->esq);
    p = rotacaoDireita(p);
    return p;
}

/* 
    Recebe o nó desbalanceado e retorna a nova raiz balanceada
*/
Arv* balanceamento(Arv* a){
    int fb = fatorB(a);
    //printf("fb =%d\n",fb);
    
    //Direita
    if(a->dir != NULL){
        //rotação simples
        if(fb == 2 && fatorB(a->dir) == 1){          //sinais positivos rotação a esquerda
            a = rotacaoEsquerda(a);
        }
        else if(fb == -2 && fatorB(a->dir) == -1){  //sinais negativos rotação a esquerda
            a = rotacaoDireita(a);
        //rotação dupla
        }
        else if(fb == -2 && fatorB(a->dir) == 1){   //sinal negativo e positivo rotação a esquerda e dps a direita
            a = rotacaoEsqDir(a);
        }
        else if(fb == 2 && fatorB(a->dir) == -1){   //sinal positivo e negativo rotação a direita e dps a esquerda
            a = rotacaoDirEsq(a);
        }
    }

    //Esquerda
    if(a->esq != NULL){
        //rotação simples
        if(fb == 2 && fatorB(a->esq) == 1){         //sinais positivos rotação a esquerda
            a = rotacaoEsquerda(a);
        }
        else if(fb == -2 && fatorB(a->esq) == -1){ //sinais negativos rotação a esquerda
            a = rotacaoDireita(a);
        //rotação dupla
        }
        else if(fb == -2 && fatorB(a->esq) == 1){  //sinal negativo e positivo rotação a esquerda e dps a direita
            a = rotacaoEsqDir(a);
        }
        else if(fb == 2 && fatorB(a->esq) == -1){  //sinal positivo e negativo rotação a direita e dps a esquerda
            a = rotacaoDirEsq(a);
        }
    }
    
    return a;
}

/*
    Insere um novo nó na arvore
*/
Arv* arv_insere(Arv* a, Jogo *chave){
    if(a == NULL){                           //arvore vazia
        a =(Arv*)malloc(sizeof(Arv));
        a->jogo = chave;
        a->esq = NULL;
        a->dir = NULL;
    }else if (strcmp(chave->id,  a->jogo->id) == -1){      //se o elemento a ser inserido é menor q o pai 
        a->esq = arv_insere(a->esq, chave);  //insere esquerda
    }else{
        a->dir = arv_insere(a->dir, chave);  //se não insere a direita
    }

    a = balanceamento(a);
    //return balanceamento(a);
    return a;
}

Arvore* insere(Arvore* arv, Jogo *chave){
    arv->raiz = arv_insere(arv->raiz, chave);
    return arv;
}

/*
    Remove um nó da arvore
    retorna a raiz pq se for necessario rebalancear a arvore e a raiz podem ser alteradas
*/
Arv* arv_remove(Arv* a, char *chave){
    if(!arv_vazia(a)){
        if (strcmp(a->jogo->id, chave) == 0){  //a->jogo->game == chave
            //Remove elemento sem filhos
            if(a->esq == NULL && a->dir == NULL){
                free(a);
                printf("\tElemento %s removido! \n", chave);
                return NULL;
            }else{
                //Remove elemento com 2 filhos
                if (a->esq != NULL && a->dir != NULL){
                    Arv *aux = a->esq;
                    while (aux->dir != NULL)
                        aux =aux->dir;
                    Jogo *jogoAux;
                    jogoAux = a->jogo;
                    a->jogo = aux->jogo;
                    aux->jogo = jogoAux;
                    printf("\tElemento %s removido! \n", chave);
                    a->esq = arv_remove(a->esq, chave);
                    return a;
                }
                //Remove elemento com 1 filho
                else{
                    Arv *aux;
                    if(a->esq != NULL)
                        aux =a->esq;
                    else
                        aux =a->dir;
                    free(a);
                    printf("\tElemento %s removido! \n", chave);
                    return aux;
                }
            }
        }else{
            if (strcmp(chave, a->jogo->id) == -1)// (chave < a->jogo->id)
                a->esq = arv_remove(a->esq, chave);
            else
                a->dir = arv_remove(a->dir, chave);
        }   
       
        a = balanceamento(a);
        return a;
    }else{
        printf("\tValor n encontrado \n");
        return NULL;
    }
}

Arvore* remover(Arvore* arv, char *chave){
    arv->raiz = arv_remove(arv->raiz, chave);
    return arv;
}

/*
    Imprime 
*/
void imprimirJogo(Jogo *jogo){
    printf("Id: %s Game: %s Series: %s Country: %s Ban Status: %s  Genre: %s \n", jogo->id, jogo->game, jogo->series, jogo->country, jogo->banStatus, jogo->genre );    
    //Id,Game,Series,Country,Details,Ban Category,Ban Status,Wikipedia Profile,Image,Summary,Developer,Publisher,Genre,Homepage
}

void imprime(Arv* arv, int h){
    int i;
    if(arv){
        imprime(arv->dir, h+1);
        printf("\n\n");

        for (i = 0; i < h; i++)
        {
            printf("\t");
        }
        
        imprimirJogo(arv->jogo);
        imprime(arv->esq, h + 1);
    }
}

/*  
    Imprime Digraph
*/
Arv* arv_imprimeD(Arv* a){
    if(!arv_vazia(a) && a->esq != NULL){                        //se a arvore não estiver vazia e o elemento a esquerda não for vazio 
        printf("\t\"%s\" -> \"%s\"\n",a->jogo->id, a->esq->jogo->id);   //imprime o elemento pai -> elemento filho a esquerda
        arv_imprimeD(a->esq);
    }
    if(!arv_vazia(a) && a->dir != NULL){                        //se a arvore não estiver vazia e o elemento a direita não for vazio 
        printf("\t\"%s\" -> \"%s\"\n",a->jogo->id, a->dir->jogo->id);   //imprime o elemento pai -> elemento filho a direita 
        arv_imprimeD(a->dir);
    }
}

Arv* imprimeDigraph(Arv* a){
    printf("digraph G{\n");
    arv_imprimeD(a);
    printf("}\n");
}

/*
    Libera a arvore
*/
Arv* arv_libera(Arv* a){
    if(!arv_vazia(a)){       //se ela não está vazia
        arv_libera(a->esq);  //libera arv a esquerda
        arv_libera(a->dir);  //libera arv a direita
        free(a);
    }
}

Arv* libera(Arvore* r){
    arv_libera(r->raiz);
    free(r);
}

/*  
    Busca elementos na arvore
*/
Arv* arv_busca (Arv* a, char *chave){
    if(arv_vazia(a)){                   
        return NULL;
    }else if(strcmp(chave, a->jogo->id) == 0){         
        printf("\tValor Encontrado! \n");
        printf("\tId: %s\n", a->jogo->id);
        printf("\tGame: %s\n", a->jogo->game);
        printf("\tCountry: %s\n", a->jogo->country);
        printf("\tBan Status: %s\n", a->jogo->banStatus);
        printf("\tGenre: %s\n", a->jogo->genre);
        //Id,Game,Series,Country,Details,Ban Category,Ban Status,Wikipedia Profile,Image,Summary,Developer,Publisher,Genre,Homepage
        
        return a;
    }else{
        Arv* b = arv_busca(a->esq, chave);  
        if(b != NULL){
            return b;
        }else{
            return arv_busca(a->dir, chave);
        }
    }
}

Arvore* busca(Arvore* arv, char *chave){
    arv->raiz = arv_busca(arv->raiz, chave);
    return arv;
}

Arvore* leArq(Arvore* arv, char* nomeArq){
    FILE* arq = fopen(nomeArq, "r");
    Jogo* jogo;
  
    while (!feof(arq))
    {
        jogo = (Jogo*)malloc(sizeof(Jogo));
        fscanf(arq, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n", jogo->id, jogo->game, jogo->series, jogo->country, jogo->banStatus, jogo->genre);
    
        arv = insere(arv, jogo);
    }

    fclose(arq);
    return arv;
}


int main(){
    int op;
    char *valor;
    Arvore* arv = arv_criavazia();
    Jogo *j;
    leArq(arv, "jogos.csv");


    do{
        printf("\n\n\t0 - Sair\n\t1 - Inserir\n\t2 - Remover\n\t3 - Imprime\n\t4 - Imprime Digraph\n\t5 - Buscar\n\n");
        scanf("%d%*c", &op);

        switch (op){
        case 0:
            printf("Saindo!");
            break;
        case 1:
            j = malloc(sizeof(Jogo));
          
            printf("\tDigite o id: ");
            scanf("%[^\n]%*c", j->id);
           
            printf("\tDigite o nome: ");
            scanf("%[^\n]%*c", j->game);

            printf("\tDigite a serie: ");
            scanf("%[^\n]%*c", j->series);

            printf("\tDigite o país: ");
            scanf("%[^\n]%*c", j->country);

            printf("\tDigite ban status: ");
            scanf("%[^\n]%*c", j->banStatus);

            printf("\tDigite genre: ");
            scanf("%[^\n]%*c", j->genre);
            
            arv = insere(arv, j);
            break;
        case 2:
            printf("\tDigite o Id a ser removido: ");
            scanf("%[^\n]%*c", valor);
            printf("\n\n");
            arv = remover(arv, valor);
            break;
        case 3:
            imprime(arv->raiz, 1);
            break;
        case 4:
            imprimeDigraph(arv->raiz);
            break;   
        case 5:
            printf("\tDigite o Id a ser buscado: ");
            scanf("%[^\n]%*c", valor);
            printf("\n\n");
            arv_busca(arv->raiz, valor);
            break;      
        default:
            printf("\nOpcao invalida!\n");
        }
    }while(op != 0);

    return 0;
}
