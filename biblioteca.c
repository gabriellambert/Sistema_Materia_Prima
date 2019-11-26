#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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
    printf("\n    ==== CADASTRO DE PRODUTOS ====\n");
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
    printf("\n    ==== REALIZAR PEDIDO ====\n");
    if (tamanho > 0) {
        if (produtos[i].pedidos_total < QTD_MAX_PEDIDOS) {
            printf("\nEntre com o codigo do produto: ");
            scanf("%d", &codigo);
            getchar();
            if ((i = procura_produto(produtos, tamanho, codigo))!= NAO_EXISTE) {

                if (produtos[i].estoque <= produtos[i].estoqueMinimo) {
                    printf("\nAtencao! O estoque atual esta abaixo do estoque minimo.");
                    pedidos[i].contador_est_min++;
                }

                printf("\nEntre com numero do pedido: ");
                scanf("%d", &pedidos[espaco].codigo_pedido);
                getchar();
                printf("\nEntre com a quantidade do pedido: ");
                scanf("%d", &pedidos[espaco].qtde_pedida);
                getchar();

                if (pedidos[espaco].qtde_pedida > pedidos[i].maior_pedido) {
                    pedidos[i].maior_pedido = pedidos[espaco].qtde_pedida;
                    pedidos[i].numero_maior = pedidos[espaco].codigo_pedido;
                }
                if (pedidos[espaco].qtde_pedida > produtos[i].estoque) {
                    printf("\nEstoque insuficiente. Nao e possivel realizar o pedido");
                    pedidos[i].pedido_recusado++;
                } else {
                    produtos[i].estoque = produtos[i].estoque - pedidos[espaco].qtde_pedida;
                    pedidos[i].soma_pedido+= pedidos[espaco].qtde_pedida;
                    produtos[i].pedidos_total++;
                    pedidos[i].preco_total+= (float)pedidos[espaco].qtde_pedida * produtos[i].preco;
                    printf("Pedido realizado");
                }
            } else {
                printf("\nProduto nao cadastrado, codigo: %d", codigo);
            }
        } else {
            printf("\nNao e possivel realizar mais pedidos. Total de %d pedidos ja realizados", QTD_MAX_PEDIDOS);
        }
    } else {
        printf("\nNao e possivel realizar pedido. Nao ha produtos cadastrados no sistema.");
    }

    return qtdePedida;
}

int entrada(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho)
{
    int i = 0;
    int codigo;
    int qtdePedida = 0;

    if (tamanho > 0) {
        if (produtos[i].entradas_total < QTD_MAX_ENTRADAS) {
            printf("\n    ==== REALIZAR ENTRADA ====\n");
            printf("\nEntre com o codigo do produto: ");
            scanf("%d", &codigo);
            getchar();

            if ((i = procura_produto(produtos, tamanho, codigo))!= NAO_EXISTE) {
                if (produtos[i].estoque >= produtos[i].estoqueMinimo) {
                    printf("\nA quantidade em estoque e maior ou igual ao estoque minimo. Entrada recusada!");
                    pedidos[i].entrada_recusada++;
                } else {
                    printf("\nEntre com a quantidade de entrada: ");
                    scanf("%d", &pedidos[espaco].qtde_entrada);
                    getchar();

                    if (produtos[i].entradas_total == 0) {
                        pedidos[i].menor_entrada = pedidos[espaco].qtde_entrada;
                    }
                    if (pedidos[espaco].qtde_entrada < pedidos[i].menor_entrada) {
                        pedidos[i].menor_entrada = pedidos[espaco].qtde_entrada;
                    }
                    produtos[i].estoque = produtos[i].estoque + pedidos[espaco].qtde_entrada;
                    pedidos[i].valor_total_entradas += (float)pedidos[espaco].qtde_entrada * produtos[i].preco;
                    pedidos[i].soma_entrada+=pedidos[espaco].qtde_entrada;
                    produtos[i].entradas_total++;
                    printf("Entrada realizada");
                }

            } else {
                printf("\nProduto nao cadastrado, codigo: %d", codigo);
            }
        } else {
           printf("\nNao e possivel realizar mais entradas. Total de %d entradas ja realizadas", QTD_MAX_ENTRADAS);
        }
    } else {
        printf("\nNao e possivel cadastrar uma entrada. Nao ha produtos cadastrados.");
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
            "08 - RELATORIO DE VENDAS\n"
            "09 - Sair do programa\n"
            "------------------------------------\n"
            "DIGITE A OPCAO DESEJADA: ");
    scanf("%d", &opcao);
    getchar();

    return opcao;
}
void exibe_produtos(tp_produto produtos[], int tamanho) {
    int i;
    printf("\n    ==== INFORMACOES DO PRODUTO ====\n");
    printf("\nCodigo \t Nome \t\t Preco \t Estoque \t Estoque Minimo \t Quantidade Pedidos \t Quantidade Entradas");
    for (i=0; i<tamanho; i++) {
        printf("\n %d\t %s\t\t %.2f\t %d\t\t %d \t\t\t %d \t\t\t %d", produtos[i].codigo, produtos[i].nome, produtos[i].preco,
               produtos[i].estoque, produtos[i].estoqueMinimo, produtos[i].pedidos_total, produtos[i].entradas_total);
    }
    return;
}

void infos_pedidos(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho) {
    int i;
    printf("\n    ==== INFORMACOES ESTATISTICAS DE PEDIDOS ====\n");

    for (i=0; i<tamanho; i++) {
        pedidos[i].media_pedidos = (float)pedidos[i].soma_pedido/(float)produtos[i].pedidos_total;
        printf("\n\n\t\t\t\t\t--- INFORMACOES DO PRODUTO: %s ---", produtos[i].nome);
        printf("\nQtde Pedidos |\t Preco Total Pedidos |\t Maior Pedido |\tMedia Pedidos |\t Abaixo do Est. Minimo |\t Pedidos Recusados");
        printf("\n %d\t\t %.2f\t\t\t %d - %d\t\t %.1f\t\t %d\t\t\t\t %d",
               produtos[i].pedidos_total,
               pedidos[i].preco_total,
               pedidos[i].numero_maior,
               pedidos[i].maior_pedido,
               (float)pedidos[i].soma_pedido/(float)produtos[i].pedidos_total,
               pedidos[i].contador_est_min,
               pedidos[i].pedido_recusado);
    }
    return;
}

void infos_entradas(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho) {
    int i;
    printf("\n    ==== INFORMACOES ESTATISTICAS DE ENTRADAS ====\n");

    for (i=0; i<tamanho; i++) {
        pedidos[i].media_entradas = (float)pedidos[i].soma_entrada/(float)produtos[i].entradas_total;
        printf("\n\n\t\t\t\t\t--- INFORMACOES DO PRODUTO: %s ---", produtos[i].nome);
        printf("\nQtde Entradas |\t Preco Total Entradas |\t Menor Entrada | Media Entradas | Entradas Recusadas");
        printf("\n %d\t\t %.2f\t\t\t %d\t\t %.1f\t\t\t %d",
               produtos[i].entradas_total,
               pedidos[i].valor_total_entradas,
               pedidos[i].menor_entrada,
               (float)pedidos[i].media_entradas,
               pedidos[i].entrada_recusada);
    }
    return;
}

void relatorio_vendas(tp_movimentacao pedidos[], tp_produto produtos[], int espaco, int tamanho){
    int i, j;
    printf("    ==== RELATORIO GRAFICO DE VENDAS ====\n");
    for (i=0; i<tamanho; i++) {
        printf("\n%s -> ", produtos[i].nome);
        for (j=0; j<pedidos[i].soma_pedido; j++) {
            printf("|");
        }
    }
    return;
}

void ascii() {
    printf("BBBB  EEEE M   M     V     V III N   N DDD   OOO\n"
           "B   B E    MM MM     V     V  I  NN  N D  D O   O\n"
           "BBBB  EEE  M M M ---  V   V   I  N N N D  D O   O\n"
           "B   B E    M   M       V V    I  N  NN D  D O   O\n"
           "BBBB  EEEE M   M        V    III N   N DDD   OOO \n");
}
