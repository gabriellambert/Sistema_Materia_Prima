#include<stdio.h>
#include<stdlib.h>
#define NAO_EXISTE -1
#define TAM_NOME_PRODUTO 50

#define ERRO_NA_CRIACAO_DE_ARQUIVO   -1
#define ERRO_NA_ABERTURA_DE_ARQUIVO  -2
#define ERRO_NA_LEITURA_DE_ARQUIVO   -3
#define ERRO_NA_GRAVACAO_DE_ARQUIVO  -4
#define REGISTRO_NAO_ENCONTRADO      -5
#define REGISTRO_EXISTENTE        -6
#define SUCESSO_OPERACAO              0

#define NOME_ARQUIVO_PRODUTOS         "produtos.dat"

typedef struct produtos
{
    int codigo;
    char nome[TAM_NOME_PRODUTO+1];
    float preco;
    int estoque;
    int estoqueMinimo;
    int pedidos_total;
    int entradas_total;
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
    int menor_entrada;
    int numero_maior;
    float media_pedidos;
    float media_entradas;
    int contador_est_min;
    int pedido_recusado;
    int entrada_recusada;
    float valor_total_entradas;
} tp_movimentacao;

int menu();
void infos();
int fazer_pedido();
int cadastrar();
int procura_produto(tp_produto produtos[], int tamanho, int codigo);
int encontra_produto(int codigo);
int entrada();
int exibe_produtos();
void infos_pedidos(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho);
void infos_entradas(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho);
void relatorio_vendas(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho);
void imprime_mensagem_erro_arquivo(int erro, char *nome_arquivo);
int cria_arquivo_produtos();
