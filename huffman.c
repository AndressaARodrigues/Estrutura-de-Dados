#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h> //para aceitar acentuação
#include <math.h>

//algoritmo baseado em https://wagnergaspar.com/testando-nosso-algoritmo-de-huffman-codigo-de-huffman/
                                 	
/***************************************************************
*											  		           *
*	Andressa De Almeida Rodrigues			   		           *
*	Universidade Federal do Pampa (UNIPAMPA)   		           *
*	Bacharelado em Ciência da Computação	   		           *
*	Organização de Arquivos e Dados - algoritmo de Huffman     *
*													           *
*	Obs: lista para armazenar frequências, matriz de caracter  *
*   para armazenar codigos, insertion sort para ordenação.     *
*                               				               *	
*	Melhorias: salvar frequência, leitura de arquivo           *
*   comprimido, imprimir arvore(graphviz) e menu.              *    
*   										  		           *
***************************************************************/

#define N 256 // tamanho

typedef struct nodo Nodo;
typedef struct lista Lista;
//'unsigned char' para o intervalo de 0 a 255 bytes
typedef unsigned char byte;
typedef unsigned char texto;
typedef unsigned int tabela_frequencia;


/*
    Definição do nodo da lista e arvore
*/
struct nodo
{
    int freq; // frequência
    byte c;   // caracter
    /*
     struct nodoArvore *esq, *dir;
     struct nodoLista *prox;
    */
    struct nodo *esq, *dir, *prox;
};

struct lista
{
    Nodo *raiz;
    int n; // tamanho
};

/*
    Inicializa tabela de frequência com zero e
    depois preenche ela
*/
void cria_tabela_freq(texto t[], tabela_frequencia tab[])
{
    int i = 0, j = 0;
    while (j < N)
    {
        tab[j] = 0;
        j++;
    }
    while (t[i] != '\0') //final string
    {
        tab[t[i]]++;              //t[i] num q corresponte tabela ascii
        i++;
    }
}

/*
    Imprime o resultado da tabela de frequência
*/
void imprime_freq(tabela_frequencia tab[])
{
    printf("\tTABELA DE FREQUÊNCIA\n");
    int i = 0;

    while (i < N)
    {
        if (tab[i] > 0)                //imprime resultados q forem maior q 0
        {
            printf("\t%d -> %d -> %c\n", i, tab[i], i);
        }
        i++;
    }
}

/*
    Cria lista vazia
*/
void cria_lista(Lista *li)
{
    li->raiz = NULL;
    li->n = 0;
}

/*
    Verifica se a lista esta vazia
*/
int lista_vazia(Lista *lista)
{
    return (lista->raiz == NULL);
}

/*
    Exibe mensagem de erro de alocação de memória
*/
void erro_arquivo()
{
    printf("\tERRO AO ABRIR ARQUIVO\n");
    exit(0);
}

/*
    Insere na lista ordenadamente
*/
void insere_lista(Lista *li, Nodo *nodo)
{
    if (lista_vazia(li)) // Se a lista pasada como parâmetro esta vazia, insere um nó no início
    {
        li->raiz = nodo;
    }

    else if (nodo->freq < li->raiz->freq) // Se tem frequência menor que o início da lista, inclui o novo nó como inicio e coloca o inicio antigo como proximo desse novo
    {
        nodo->prox = li->raiz;
        li->raiz = nodo;
    }
    else
    {
        Nodo *aux = li->raiz->prox;
        Nodo *aux2 = li->raiz;

        while (aux && aux->freq <= nodo->freq) // Laço percorre a lista e insere o nó na posição certa de acordo com sua frequência
        {
            aux2 = aux;
            aux = aux2->prox;
        }
        aux2->prox = nodo;
        nodo->prox = aux;
    }
    li->n++;
}

/*
    Preenche a lista
*/
void preenche_lista(Lista *li, tabela_frequencia tab[])
{
    int i = 0;
    while (i < N)
    {
        if (tab[i] > 0)
        {
            Nodo *novo = (Nodo *)malloc(sizeof(Nodo));
            if (novo == NULL)
            {
                erro_arquivo();
            }
            else
            {
                novo->c = i;
                novo->freq = tab[i];
                novo->dir = novo->esq = novo->prox = NULL;

                insere_lista(li, novo);
            }
        }
        i++;
    }
}

/*
    Remove a lista
*/
Nodo *remove_lista(Lista *li)
{
    Nodo *aux = NULL;

    if (!lista_vazia(li))
    {
        aux = li->raiz;
        li->raiz = aux->prox;
        aux->prox = NULL;
        li->n--;
    }
    return aux;
}

/*
    Cria a árvore
*/
Nodo *cria_arvore(Lista *li)
{
    Nodo *nodo1, *nodo2, *novo; //remove de 2 em 2
    while (li->n > 1)
    {
        nodo1 = remove_lista(li);
        nodo2 = remove_lista(li);
        novo = (Nodo *)malloc(sizeof(Nodo));

        if (novo == NULL)
        {
            erro_arquivo();
        }
        else
        {
            novo->freq = nodo1->freq + nodo2->freq;
            novo->esq = nodo1;
            novo->dir = nodo2;
            novo->prox = NULL;

            insere_lista(li, novo);
        }
    }
    return li->raiz;
}

/*
    Retorna a altura da árvore
*/
int max_arvore(int esq, int dir)
{
    return (esq > dir) ? esq : dir;
}

int altura_arvore(Nodo *raiz)
{
    if (raiz != NULL) // if(!lista_vazia(raiz))
    {
        return 1 + max_arvore(altura_arvore(raiz->esq),
                              altura_arvore(raiz->dir));
    }
    else
        return -1;
}
/*
    Verifica se é uma folha
*/
int eh_folha(Nodo* aux)
{
    return(aux->esq == NULL && aux->dir == NULL);
}

/*
    Aloca Memória para tradução
*/
char **inicia_tradutor(int col)
{
    char **tradutor = malloc(sizeof(char *) * N);

    int i = 0;
    while (i < N)
    {
        tradutor[i] = calloc(col, sizeof(char));
        i++;
    }
    return tradutor;
}

/*
    Cria tradutor
*/
void cria_tradutor(Nodo *raiz, int col, char **tradutor, char *d)
{
    char esq[col], dir[col];

    if (eh_folha(raiz)){ //folha
        strcpy(tradutor[raiz->c], d);
    }
    else
    {
        strcpy(esq, d); //se for para esquerda concatena 0
        strcat(esq, "0");
        strcpy(dir, d); //se for para direita concatena 1
        strcat(dir, "1");

        cria_tradutor(raiz->esq, col, tradutor, esq);
        cria_tradutor(raiz->dir, col, tradutor, dir);
    }
}

/*
    Imprime Dicionário
*/
void imprime_tradutor(char **tradutor)
{
    printf("\n\tTRADUÇÃO\n");
    int i = 0;
    while (i < N)
    {
        if (strlen(tradutor[i]) > 0) //strlen verifica tamanho
            printf("\t%d -> %s\n", i, tradutor[i]);
        i++;
    }
}

// quantidade de caracteres de um texto
int tamanho_texto(FILE *arq, char *nomeArquivo)
{
    arq = fopen(nomeArquivo, "r"); //
    int n = 0;                               // tamanho

    if (arq == NULL)
    {
        erro_arquivo();
    }
    else
    {
        while (fgetc(arq) != -1)
            n++;
        fclose(arq);
    }
    return n;
}

/*
    Leitura do texto de um arquivo
*/
void leitura_arquivo(FILE *arq, texto *texto, char *nomeArquivo) /* */
{
    arq = fopen(nomeArquivo, "r");
    unsigned char c;

    if (arq == NULL)
    {
        erro_arquivo();
    }
    else
    {
        int i = 0;
        while (!feof(arq))
        {
            c = fgetc(arq);
            if (c != -1)
            {
                texto[i] = c;
                i++;
            }
        }
        fclose(arq);
    }
}

/*
    COMPRESSÃO
*/
char *stringCodificao(texto *t, char **tradutor)
{
    int i = 0, n = 0, j = 0;
    while (t[i] != '\0')
    {
        n = n + strlen(tradutor[t[i]]); // calcula tamanho da string
        i++;
    }

    char *cod = calloc(n + 1, sizeof(char));
    while (t[j] != '\0')
    {
        strcat(cod, tradutor[t[j]]); //concatena strings
        j++;
    }
    return cod;
}

void comprimir(FILE *arq, texto str[], char *nomeArquivo)
{
    arq = fopen(nomeArquivo, "wb"); //'wb' escrita binária, se ele n existir vai ser criado
    texto mask, byte = 0;
    int i = 0, j = 7;


    if (arq == NULL)
    {
        erro_arquivo();
    }
    // oito caracteres são convertidos em oito bits
    else
    {
        while (str[i] != '\0')
        { // se o elemento é diferente do final da string
            mask = 1;
            if (str[i] == '1')
            {
                mask = mask << j; // deslocamento << 7
                byte = byte | mask;  // operação OU
            }
            j--;

            if (j < 0)
            { // um byte formado escreve no arquivo
                fwrite(&byte, sizeof(texto), 1, arq);
                byte = 0; // recomeça
                j = 7;
            }

            i++;
        }           // existe um bit pela metade
        if (j != 7) // um byte n finalizado escreve no arquivo
            fwrite(&byte, sizeof(texto), 1, arq);
        fclose(arq);
    }
}

/*
    DESCOMPRESSÃO
 */
char *stringDecodificao(Nodo *raiz, texto t[])
{
    Nodo *aux = raiz;
    char aux2[2];
    char *dcod = calloc(strlen(t), sizeof(char));

    int i = 0;
    while (t[i] != '\0')
    {
        if (t[i] == '1')
        {
            aux = aux->dir;
        }
        else
        {
            aux = aux->esq;
        }
        if (eh_folha(aux)) //chegou em folha
        {
            aux2[0] = aux->c;
            aux2[1] = '\0';
            strcat(dcod, aux2);
            aux = raiz;
        }
        i++;
    }
    return dcod;
}

void descomprimir(FILE *arq, Nodo *raiz, char *nomeArquivo)
{
    arq = fopen(nomeArquivo, "rb"); //
    FILE *arquivo = fopen("descomprimido.txt", "wb"); //

    Nodo *aux = raiz;
    texto byte, mask;

    if (arq == NULL)
    {
        erro_arquivo();
    }
    else
    {
        while (fread(&byte, sizeof(texto), 1, arq)) //tenta ler e se conseguir salva na variavel e retorna quantidade de elementos do tipo
        {
            for (int i = 7; i >= 0; i--)
            {
                mask = (1 << i);
                //operacao AND
                if (byte & mask){ //qnd 1 caminha dir

                    aux = aux->dir;
                }
                else{
                    aux = aux->esq;
                }//folha
                if (eh_folha(aux))
                {
                    fwrite(&aux->c, sizeof(texto), 1, arquivo);
                    aux = raiz; //volta p raiz e recomeça
                }
            }
        }
        fclose(arquivo);
    }
}


int main()
{
    Lista lista;
    texto *texto;
    tabela_frequencia tab_freq[N];
    int colunas;
    char **tradutor;
    FILE *arquivo;

    setlocale(LC_ALL, "Portuguese");

    printf("\n\tCOMPRESSÃO E DESCOMPRESSÃO DE HUFFMAN\n\n");

    int tam = tamanho_texto(arquivo, "teste.txt"); //descobre quantidade de caracteres
    texto = calloc(tam + 2, sizeof(texto)); //
    leitura_arquivo(arquivo, texto, "teste.txt");

    cria_tabela_freq(texto, tab_freq);

    cria_lista(&lista);
    preenche_lista(&lista, tab_freq);

    Nodo *arvore = cria_arvore(&lista);

    tradutor = inicia_tradutor(colunas);
    cria_tradutor(arvore, altura_arvore(arvore) + 1, tradutor, "");

    char *codificar = stringCodificao(texto, tradutor);      // texto codificado
    comprimir(arquivo, codificar, "comprimido.txt");
    printf("\n\tARQUIVO FOI COMPRIMIDO!");
    printf("\n\tQuantidade de caracteres no arquivo comprimido: %d\n\n", tamanho_texto(arquivo, "comprimido.txt"));

    char *decodificar = stringDecodificao(arvore,codificar); // texto decodificado
    descomprimir(arquivo, arvore, "comprimido.txt");
    printf("\n\tARQUIVO FOI DESCOMPRIMIDO!");
    printf("\n\tQuantidade de caracteres no arquivo descomprimido: %d\n\n", tamanho_texto(arquivo, "teste.txt")+1);


    printf("\n\t Verifique os arquivos!\n");

    free(texto);
    free(codificar);
    free(decodificar);

    return 0;
}
