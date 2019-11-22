#include<stdio.h>
#include<stdlib.h>
#define QTD_MAX_PRODUTOS 5
#define QTD_MAX_PEDIDOS  3
#define QTD_MAX_ENTRADAS  10
#define NAO_EXISTE -1
#define TAM_NOME_PRODUTO 50

typedef struct produtos
{
    int codigo;
    char nome[TAM_NOME_PRODUTO+1];
    float preco;
    int estoque;
    int estoqueMinimo;
} tp_produto;

typedef struct pedidos
{
    int pedido_maximo;
    int numero_pedido;
    int qtde_pedida;
    int qtde_entrada;
    int soma_entrada;
    int codigo_pedido;
    int codigo_entrada;
    int soma_pedido;
    float preco_total;
    int maior_pedido;
    int maior_entrada;
    int numero_maior;
    float media_pedidos;
    int contador_est_min;
    int pedido_recusado;
    int entrada_recusada;
} tp_movimentacao;

int menu();
void infos();
int fazer_pedido(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho);
int cadastrar(tp_produto produtos[], int tamanho);
int procura_produto(tp_produto produtos[], int tamanho, int codigo);
void exibe_produtos(tp_produto produtos[], int tamanho);
int entrada(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho);



