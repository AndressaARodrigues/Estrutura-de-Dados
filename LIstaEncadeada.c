#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*****************************************************
*		 	                             *
*	Andressa De Almeida Rodrigues	             *
*	Universidade Federal do Pampa (UNIPAMPA)     *
*	Bacharelado em Ciência da Computação	     *
*	Estrutura de Dados 1 - Lista Encadeada       *
*       	  	                             *
******************************************************/

//Obs: Possui erro ao remover					               
//Melhorias: Busca(Nome), Busca(Case-sensitive) e Alocação Dinamica.

struct cliente
{
	int codigo;
	char nome[40];
	float saldo;
	struct cliente *prox;
};
typedef struct cliente listaCliente;

/*
	Função de inicialização: retorna uma lista vazia
*/
listaCliente *inicializaLista()
{
	return NULL;
}

/*
	Cadastra Cliente: cadastra ordenadamente
*/
listaCliente *cadastrarCliente(listaCliente *cli, int cod, char nome[40], float sal)
{

	listaCliente *novo = malloc(sizeof(listaCliente));
	listaCliente *ant = NULL; /* ponteiro para elemento anterior */
	listaCliente *aux = cli;  /* ponteiro para percorrer a lista*/

	novo->codigo = cod;
	strcpy(novo->nome, nome);
	novo->saldo = sal;

	/* procura posição de inserção */
	while (aux != NULL && aux->codigo < cod)
	{
		ant = aux;
		aux = aux->prox;
	}
	/* insere elemento */
	if (ant == NULL)
	{ /* insere elemento no início */
		novo->prox = cli;
		cli = novo;
	}
	else
	{ /* insere elemento no meio da lista */
		novo->prox = ant->prox;
		ant->prox = novo;
	}
	return cli;
}

/*
	Inserção no início: retorna a lista atualizada
*/
listaCliente *insereInicio(listaCliente *cli, int cod, char name[40], float sal)
{
	listaCliente *novo = malloc(sizeof(listaCliente));

	novo->codigo = cod;
	strcpy(novo->nome, name);
	novo->saldo = sal;

	if (cli == NULL)
	{
		novo->prox = NULL;
		return novo;
	}
	else
	{
		novo->prox = cli;
		cli = novo;
	}
	return cli;
}

/*
	Função imprime: imprime valores dos elementos
*/
void imprimeLista(listaCliente *cli)
{

	if (cli == NULL)
	{
		printf("A lista esta vazia \n");
	}
	else
	{
		listaCliente *aux = cli; /* variável auxiliar para percorrer a lista */
		while (aux != NULL)
		{
			printf("\tId: %d \n", aux->codigo);
			printf("\tNome: %s", aux->nome);
			printf("\tSaldo: %2.f \n\n", aux->saldo);
			aux = aux->prox;
		}
	}
}

/*
	Função vazia: retorna 1 se vazia ou 0 se não vazia
*/
int vazia(listaCliente *cli)
{
	if (cli == NULL)
		return 1;
	else
		return 0;
}

/*
	Função busca: busca um elemento na lista
*/
listaCliente *buscaCod(listaCliente *cli, int codigo)
{
	listaCliente *aux = cli;
	while (aux != NULL && aux->codigo != codigo)
	{
		aux = aux->prox;
	}
	return aux;
}

/*
	Função retira: retira elemento da lista
*/
listaCliente *retira(listaCliente *cli, int cod)
{
	listaCliente *aux = cli; /* ponteiro para percorrer a lista*/
	listaCliente *ant;		 /* ponteiro para elemento anterior */

	/* procura elemento na lista, guardando anterior */
	while (aux != NULL && cod != aux->codigo)
	{
		ant = aux;
		aux = aux->prox;
	}
	/* verifica se achou elemento */
	if (aux == NULL)
		return cli; /* não achou: retorna lista original */

	/* retira elemento */
	if (ant == NULL)
	{
		/* retira elemento do inicio */
		cli = aux->prox;
	}
	else
	{
		/* retira elemento do meio da lista */
		ant->prox = aux->prox;
		free(aux);
		return cli;
	}
}

listaCliente *libera(listaCliente *cli)
{
	listaCliente *aux = cli;
	listaCliente *ant;

	while (aux != NULL)
	{

		ant = aux->prox; /* guarda referência para o próximo elemento*/
		free(aux);		 /* libera a memória apontada por aux*/
		aux = ant;		 /* faz aux apontar para o próximo */
	}
	return aux;
}

int lerMenu()
{
	int x;
	printf("\n\tCadastro de Clientes\n");
	printf("\n\t0 | Sair\n\t1 | Cadastrar Cliente\n\t2 | Remover Cliente\n\t3 | Pesquisar Cliente(Id)\n\t4 | Exibir Clientes\n\n");
	printf(">>>");
	scanf("%d", &x);
	return x;
}

int main(void)
{
	listaCliente *cli; /* declara uma lista não iniciada */
	listaCliente *d;

	int op;
	int codigo;
	char nome[40];
	float saldo;

	cli = inicializaLista(); // inicia lista vazia

	do
	{
		op = lerMenu();

		if (op == 0)
			break; // sair do programa

		switch (op)
		{
		case 1:
			printf("\tCadastro ...\n");
			printf("\tCodigo >> ");
			scanf("%d%*c", &codigo);
			printf("\tNome >>> ");
			fgets(nome, 40, stdin);
			// scanf("%[^\n]%*c", nome);
			printf("\tSaldo >>> ");
			scanf("%f", &saldo);
			printf(" \n");

			cli = cadastrarCliente(cli, codigo, nome, saldo);

			break;
		case 2:
			printf("\tRemover ...\n");
			printf("\tCodigo >>> ");
			scanf("%d", &codigo);
			cli = retira(cli, codigo);
			break;
		case 3:
			printf("\tBuscar... \n");
			printf("\tCodigo >>> ");
			scanf("%d", &codigo);
			d = buscaCod(cli, codigo);

			if (d == NULL)
			{
				printf("\tValor nao encontrado! \n\n");
			}
			else
			{
				printf("\tDados encontrados! \n\n");
				printf("\tId: %d \n", d->codigo);
				printf("\tNome %s ", d->nome);
				printf("\tSaldo %2.f \n\n", d->saldo);
			}
			break;
		case 4:
			printf("\tExibindo...\n");
			imprimeLista(cli);
			break;

		default:
			printf("\tValor invalido! \n");
		}

	} while (op != 0 && op < 5);

	printf("\tSaindo do cadastro...");

	libera(cli);

	return 0;
}
