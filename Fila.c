#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************
*											  		                      *
* 	Andressa De Almeida Rodrigues			   		      *
* 	Universidade Federal do Pampa (UNIPAMPA)      *
*	  Bacharelado em Ciência da Computação	   	    *
*	  Estrutura de Dados 1 - Fila             	    *				
*											                            *
***************************************************/

//Melhorias: Busca(Nome), Busca(Case-sensitive) e Alocação Dinamica.	

typedef struct cliente
{
    char nome[40];
    char codigo[5];
    struct cliente *prox;
} Cliente;

/*
	Função de inicialização: retorna uma fila vazia
*/
Cliente *iniciaFila()
{
    return NULL;
}

Cliente *enfileirar(Cliente *fila, char codigo[5], char nome[40])
{
    Cliente *novo = malloc(sizeof(Cliente));
    Cliente *aux = fila;
    strcpy(novo->codigo, codigo);
    strcpy(novo->nome, nome);
    novo->prox = NULL;
    if (fila == NULL)
    {
        return novo;
    }
    else
    {
        while (aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = novo;
    }
    return fila;
}

Cliente *lerArquivo(Cliente *fila, char nomeARquivo[20])
{

    Cliente cliente;

    FILE *arquivo = fopen(nomeARquivo, "r");
    fscanf(arquivo, "Codigo, Nome");

    while (fscanf(arquivo, " %[^,] ,%[^\n] \n", cliente.codigo, cliente.nome) == 2)
    {
        //printf("Codigo: %s Nome: %s \n", cliente.codigo, cliente.nome); //para ver se esta lendo o arquivo
        fila = enfileirar(fila, cliente.codigo, cliente.nome);
    }
    fclose(arquivo);
    return fila;
}

void imprimirFila(Cliente *fila)
{
    Cliente *aux = fila;
    while (aux != NULL)
    {
        printf("\tCodigo: %s Nome: %s\n", aux->codigo, aux->nome);
        aux = aux->prox;
    }
}

void gravarArquivo(Cliente *gravar)
{
}

Cliente *insereInicioFila(Cliente *fila, char nome[40], char codigo[5])
{

    Cliente *novo = malloc(sizeof(Cliente));

    strcpy(novo->codigo, codigo);
    strcpy(novo->nome, nome);

    if (fila == NULL)
    {
        return novo;
    }
    else
    {
        novo->prox = fila;
        fila = novo;
        return fila;
    }
}

Cliente *desenfileirar(Cliente *fila)
{
    Cliente *aux = fila;
    Cliente *prox = NULL;
    prox = aux->prox;
    fila = prox;
    free(aux);
    return fila;
}

Cliente *buscarCliente(Cliente *fila, char codigo[5])
{
    Cliente *aux = fila;
    while (aux != NULL && strcmp(aux->codigo, codigo) != 0)
    {
        aux = aux->prox;
    }
    return aux;
}

int vazia(Cliente *fila)
{
    if (fila == NULL)
        return 1;
    else
        return 0;
}

int lerMenu()
{
    int x;
    printf("\n\tAtendimento\n");
    printf("\n\t0 | Sair\n\t1 | Enfileirar\n\t2 | Exibir\n\t3 | Buscar(Id)\n\t4 | Desenfileirar\n\n");
    printf(">>>");
    scanf("%d", &x);
    return x;
}

int main(void)
{
    Cliente *fila = iniciaFila();  /* declara uma lista não iniciada e inicia uma lista vazia */
    Cliente *filaP = iniciaFila(); /* declara uma lista não iniciada e inicia uma lista vazia */
    Cliente *e;
    Cliente *d;
    int vazio;

    int op;
    char nome[40];
    char codigo[5];
    int num;
    Cliente cliente;

    filaP = lerArquivo(filaP, "ClientesFilaP.csv");
    fila = lerArquivo(fila, "ClientesFila.csv");

    do
    {
        op = lerMenu();

        if (op == 0)
            break; // sair do programa

        switch (op)
        {

        case 1:
            printf("\n\tEscolha uma fila: \n\t1- Fila Preferencial\n\t2 - Fila Normal\n");
            printf(">>>");
            scanf(" %d", &num);
            if (num == 1)
            {
                printf("\tEnfileirando ...\n\n");

                printf("\tDigite o codigo >>>");
                scanf("%s%*c", codigo);

                printf("\tDigite o nome >>>");
                scanf("%[^\n]%*c", nome);

                printf("\n");

                filaP = enfileirar(filaP, codigo, nome);
                //->gravar
            }
            else if (num == 2)
            {
                printf("\tEnfileirando ...\n\n");

                printf("\tDigite o codigo: >>>");
                scanf("%s%*c", codigo);

                printf("\tDigite o nome: >>>");
                scanf("%[^\n]%*c", nome);

                printf("\n");

                fila = enfileirar(fila, codigo, nome);
                //->gravar
            }
            break;
        case 2:
            printf("\tExibindo...\n\n");
            printf("\t\tFILA PREFERENCIAL\n");
            imprimirFila(filaP);
            printf("\n\t\tFILA NORMAL\n");
            imprimirFila(fila);
            printf("\n");

            break;
        case 3:
            printf("\tBusca ...\n\n");

            printf("\tQual sua busca? >>>");
            scanf("%s%*c", codigo);

            e = buscarCliente(filaP, codigo);
            d = buscarCliente(fila, codigo);

            if (e == NULL && d == NULL)
            {
                printf("\tValor nao encontrado na Fila! \n \n");
            }
            else
            {
                printf("\tDados encontrados na Fila! \n\n");
            }

            printf("\n");

            break;
        case 4:

            printf("\tDesenfileirando ...\n");
            vazio = vazia(filaP);
            if (vazio == 0)
                filaP = desenfileirar(filaP);
            else if (vazio == 1)
                fila = desenfileirar(fila);
            printf("\n");

            //-> gravar
            break;

        default:
            printf("\tValor invalido!\n");
        }
    } while (op != 0 && op < 5);

    printf("\tSaindo do cadastro...");
    return 0;
}
