
/* Sistema de gestão de produtos, com funcionalidades de cadastro; entrada; pedido; informações de produtos, pedidos e entradas e;
relatorio gráfico de vendas.

Curso: Ciência da COmputação - PUC-MG, Poços de Caldas
Matérial: Algoritmos e estrutura de dados 01


Autores: Gabriel M. Lambert e Pedro Henrique Ronchini
*/

#include <stdio.h>
#include <stdlib.h>
#include"biblioteca.h"
#include "rlutil.h"

int main() {


    setBackgroundColor(WHITE);
    setColor(BLACK);
    saveDefaultColor();
    cls();

    tp_produto Produtos = {0};
    tp_movimentacao pedidos = {0};

    int quantidade_produtos = 0, quantidade_pedidos = 0, quantidade_entradas= 0;
    int opcao=1, status;

    ascii();

    if ((status=cria_arquivo_produtos()) != SUCESSO_OPERACAO)
    {
        imprime_mensagem_erro_arquivo(status, NOME_ARQUIVO_PRODUTOS);
        exit(status);
    }

    do {

        opcao=menu();

        cls();
        switch(opcao) {

        case 2: //Cadastrar produtos
            setColor(BLUE);

            if ((status = cadastrar()) != SUCESSO_OPERACAO) {
                imprime_mensagem_erro_arquivo(status, NOME_ARQUIVO_PRODUTOS);
            }

            resetColor();
            break;

        case 3: //Realizar pedidos
            setColor(GREEN);
            if ((status = fazer_pedido()) != SUCESSO_OPERACAO) {
                imprime_mensagem_erro_arquivo(status, NOME_ARQUIVO_PRODUTOS);
            }
            //quantidade_pedidos = fazer_pedido(pedidos, Produtos, quantidade_pedidos, quantidade_produtos);
            resetColor();
            break;

        case 4: //Realizar entradas de produtos
            setColor(GREEN);
            if ((status = entrada()) != SUCESSO_OPERACAO) {
                imprime_mensagem_erro_arquivo(status, NOME_ARQUIVO_PRODUTOS);
            }
            //quantidade_entradas = entrada(pedidos, Produtos, quantidade_entradas, quantidade_produtos);
            resetColor();
            break;

        case 5: //Exibir informacoes
            setColor(RED);
            if ((status = exibe_produtos()) != SUCESSO_OPERACAO) {
                imprime_mensagem_erro_arquivo(status, NOME_ARQUIVO_PRODUTOS);
            }
            //exibe_produtos(Produtos, quantidade_produtos);
            resetColor();
            break;

        case 6: //Informacoes de realizacao de pedidos
            setColor(GREEN);
            //infos_pedidos(pedidos, Produtos, quantidade_entradas, quantidade_produtos);
            resetColor();
            break;

        case 7: //Informacoes de realizacao de entradas
            setColor(GREEN);
            //infos_entradas(pedidos, Produtos, quantidade_entradas, quantidade_produtos);
            resetColor();
            break;

        case 8: //Relatorio de vendas
            setColor(GREEN);
            //relatorio_vendas(pedidos, Produtos, quantidade_entradas, quantidade_produtos);
            resetColor();
            break;

        default:
            setColor(RED);
            printf("Opcao invalida!");
            resetColor();
        }
    } while (opcao!=9);

    return (0);
}
