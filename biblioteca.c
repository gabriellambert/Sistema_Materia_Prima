#include <stdio.h>
#include <stdlib.h>
#include"biblioteca.h"

int procura_produto(tp_produto produtos[], int tamanho, int codigo) {
    int posicao = NAO_EXISTE, i;
    for (i=0; i<tamanho;i++) {
        if(produtos[i].codigo==codigo){
            posicao=i;
            break;
        }
    }
    return posicao;
}

int cadastrar(tp_produto produtos[], int tamanho) {
    //char produtos[tamanho].nome[TAM_NOME_PRODUTO] = " ";
    int codigo;
    printf("\n-- CADASTRO DE PRODUTOS --");
    if (tamanho <= QTD_MAX_PRODUTOS) {

        printf("\nEntre com o codigo: ");
        scanf("%d", &codigo);
        getchar();
        if (procura_produto(produtos, tamanho, codigo)==NAO_EXISTE){
            produtos[tamanho].codigo = codigo;
            printf("Entre com o nome do produto: ");
            scanf("%50[^\n]s", produtos[tamanho].nome);
            getchar();
    //        printf("\n--> %s...", produtos[tamanho].nome);
    //        getchar();
            printf("Entre com o preco do produto: ");
            scanf("%f", &produtos[tamanho].preco);
            getchar();
            printf("Entre com a quantidade inicial do produto: ");
            scanf("%d", &produtos[tamanho].estoque);
            getchar();
            printf("Entre com o estoque minimo do produto: ");
            scanf("%d", &produtos[tamanho].estoqueMinimo);
            getchar();
            tamanho = tamanho+1;
        } else {
            printf("\nProduto com codigo %d ja existente", codigo);
        }
    } else {
        printf("Você já cadastrou a quantidade maxima, de 5 produtos permitida!");
    }

    return tamanho;
}

int fazer_pedido(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho)
{
    int i = 0;
    int codigo;
    int qtdePedida = 0;
    printf("\n-- REALIZAR PEDIDO --");
    printf("\nEntre com o codigo do produto: ");
    scanf("%d", &codigo);
    getchar();
    printf("\nEntre com numero do pedido: ");
    scanf("%d", &pedidos[espaco].codigo_pedido);
    getchar();
    printf("\nEntre com a quantidade do pedido: ");
    scanf("%d", &pedidos[espaco].qtde_pedida);
    getchar();

    if ((i = procura_produto(produtos, tamanho, codigo))!= NAO_EXISTE) {

        if (pedidos[espaco].qtde_pedida > pedidos[i].maior_pedido) {
            pedidos[i].maior_pedido = pedidos[espaco].qtde_pedida;
            pedidos[i].numero_maior = pedidos[espaco].numero_pedido;
        }
        if (pedidos[espaco].qtde_pedida > produtos[i].estoque) {
            printf("\nEstoque insuficiente");
            pedidos[i].pedido_recusado++;
        }
        produtos[i].estoque = produtos[i].estoque - pedidos[espaco].qtde_pedida;
        pedidos[espaco].soma_pedido++;
        printf("Pedido realizado");

    } else {
        printf("\nProduto nao cadastrado, codigo: %d", codigo);
    }

    return qtdePedida;
}

int entrada(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho)
{
    int i = 0;
    int codigo;
    int qtdePedida = 0;
    printf("\n-- REALIZAR PEDIDO --");
    printf("\nEntre com o codigo do produto: ");
    scanf("%d", &codigo);
    getchar();
    printf("\nEntre com a quantidade de entrada: ");
    scanf("%d", &pedidos[espaco].qtde_entrada);
    getchar();

    if ((i = procura_produto(produtos, tamanho, codigo))!= NAO_EXISTE) {

        if (pedidos[espaco].qtde_entrada > pedidos[i].maior_entrada) {
            pedidos[i].maior_pedido = pedidos[espaco].qtde_pedida;
            pedidos[i].numero_maior = pedidos[espaco].numero_pedido;
        }
        if (produtos[i].estoque >= produtos[i].estoqueMinimo) {
            printf("\nA quantidade em estoque e maior ou igual ao estoque minimo. Entrada recusada!");
            pedidos[i].entrada_recusada++;
        } else {
            produtos[i].estoque = produtos[i].estoque + pedidos[espaco].qtde_entrada;
            printf("Entrada realizada");
        }

    } else {
        printf("\nProduto nao cadastrado, codigo: %d", codigo);
    }

    return qtdePedida;
}

int menu() {
    int opcao;
    printf("\n------------------------------------"
            "\n           MENU DE OPCOES\n"
            "------------------------------------\n"
            "02 - CADASTRAR MATERIA-PRIMA\n"
            "03 - FAZER PEDIDOS\n"
            "04 - ENTRADA DE MATERIA-PRIMA\n"
            "05 - INFORMACOES DE MATERIA-PRIMA\n"
            "06 - MOVIMENTACAO DE PEDIDOS\n"
            "07 - MOVIMENTACAO DE ENTRADAS\n"
            "08 - RELATORIO NAO DEFINIDO\n"
            "09 - Sair do programa\n"
            "------------------------------------\n"
            "DIGITE A OPCAO DESEJADA: ");
    scanf("%d", &opcao);
    getchar();

    return opcao;
}
void exibe_produtos(tp_produto produtos[], int tamanho) {
    int i;
    printf("\n-- INFORMACOES DO PRODUTO --");
    printf("\nCodigo \t Nome \t Preco \t Estoque \t Estoque Minimo");
    for (i=0; i<tamanho; i++) {
        printf("\n %d\t %s\t %.2f\t %d\t\t %d", produtos[i].codigo, produtos[i].nome, produtos[i].preco, produtos[i].estoque, produtos[i].estoqueMinimo);
    }
    return;
}

void infos_pedidos(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho) {
    int i;
    printf("\n-- INFORMACOES ESTATISTICAS DE PEDIDOS --");
    printf("\nQtde Pedidos \t Preco Total Pedidos \t no / qtde maior pedido \t média das quantidades dos pedidos \t Qtde pedidos abaixo do estoque minimo \t Pedidos Recusados");
    for (i=0; i<tamanho; i++) {
        printf("\n %d\t %s\t %.2f\t %d\t\t %d", pedidos[i].soma_pedido, (float)produtos[i].preco*pedidos[i].soma_pedido, produtos[i].estoque, produtos[i].estoqueMinimo);
    }
    return;
}
