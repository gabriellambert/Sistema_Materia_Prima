#include <stdio.h>
#include <stdlib.h>
#include"biblioteca.h"

int main() {
    tp_produto Produtos[QTD_MAX_PRODUTOS] = {0};
    tp_movimentacao pedidos[QTD_MAX_PEDIDOS] = {0};

    int quantidade_produtos = 0, quantidade_pedidos = 0, quantidade_entradas= 0;
    int opcao=1;

    do {
        opcao=menu();

        switch(opcao) {
        case 2: //Cadastrar produtos
            if (quantidade_produtos<QTD_MAX_PRODUTOS) {
                quantidade_produtos = cadastrar(Produtos, quantidade_produtos);
            } else {
                printf("\nNao e possivel cadastrar mais produtos!!");
            }
            break;

        case 3: //Realizar pedidos
            quantidade_pedidos = fazer_pedido(pedidos, Produtos, quantidade_pedidos, quantidade_produtos);

            break;

        case 4: //Realizar entradas de produtos
            quantidade_entradas = entrada(pedidos, Produtos, quantidade_entradas, quantidade_produtos);

            break;

        case 5: //Exibir informacoes
            exibe_produtos(Produtos, quantidade_produtos);
            break;

        case 6: //Informacoes de realizacao de pedidos

            break;

        case 7: //Informacoes de realizacao de entradas

            break;

        case 8: //Relatorio nao definido

            break;

        default:
            printf("Opcao invalida!");
        }
    } while (opcao!=9);

    return (0);
}
