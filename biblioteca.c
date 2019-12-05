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

int encontra_produto(int codigo){
    FILE                    *arquivo_produtos;
    tp_produto     produtos;
    int                     conta_registro  = 1,
                            status = REGISTRO_NAO_ENCONTRADO;

    if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "rb")) == NULL)
        return ERRO_NA_ABERTURA_DE_ARQUIVO;

    if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0)
    {
        if (!feof(arquivo_produtos))
        {
            fclose(arquivo_produtos);
            return ERRO_NA_LEITURA_DE_ARQUIVO;
        }
    }

    while(!feof(arquivo_produtos))
    {
        if (produtos.codigo == codigo)
        {
            status = REGISTRO_EXISTENTE;
            break;
        }
        conta_registro += 1;

        if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0)
        {
            if (!feof(arquivo_produtos))
            {
                fclose(arquivo_produtos);
                return ERRO_NA_LEITURA_DE_ARQUIVO;
            }
        }
    }
    fclose(arquivo_produtos);
    return status;
}

int cadastrar() {
    FILE *arquivo_produtos;
    tp_produto produtos;

    int codigo, status;

    printf("\n    ==== CADASTRO DE PRODUTOS ====\n");


    printf("\nEntre com o codigo: ");
    scanf("%d", &codigo);
    getchar();

    status = encontra_produto(codigo);
    if (status != REGISTRO_NAO_ENCONTRADO){
        return status;
    }

        produtos.codigo = codigo;
        printf("Entre com o nome do produto: ");
        scanf("%50[^\n]s", produtos.nome);
        getchar();
//        printf("\n--> %s...", produtos[tamanho].nome);
//        getchar();
        printf("Entre com o preco do produto: ");
        scanf("%f", &produtos.preco);
        getchar();
        printf("Entre com a quantidade inicial do produto: ");
        scanf("%d", &produtos.estoque);
        getchar();
        printf("Entre com o estoque minimo do produto: ");
        scanf("%d", &produtos.estoqueMinimo);
        getchar();


    if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "ab+")) == NULL) {
        return ERRO_NA_ABERTURA_DE_ARQUIVO;
    }

    if (fwrite(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0) {
        fclose(arquivo_produtos);
        return ERRO_NA_GRAVACAO_DE_ARQUIVO;
    }

    fclose(arquivo_produtos);
    return SUCESSO_OPERACAO;
}

int exibe_produtos() {

    FILE *arquivo_produtos;
    tp_produto produtos;

    if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "rb")) == NULL){
        return ERRO_NA_ABERTURA_DE_ARQUIVO;
    }

    printf("\n    ==== INFORMACOES DO PRODUTO ====\n");
    printf("\nCodigo \t Nome \t\t Preco \t Estoque \t Estoque Minimo \t Quantidade Pedidos \t Quantidade Entradas");

    if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0){
        if (!feof(arquivo_produtos)){
            fclose(arquivo_produtos);
            return ERRO_NA_LEITURA_DE_ARQUIVO;
        }
    }
    while(!feof(arquivo_produtos)){
        printf("\n %d\t %s\t\t %.2f\t %d\t\t %d \t\t\t %d \t\t\t %d", produtos.codigo, produtos.nome, produtos.preco,
               produtos.estoque, produtos.estoqueMinimo, produtos.pedidos_total, produtos.entradas_total);

        if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0){
                if (!feof(arquivo_produtos)){
                    fclose(arquivo_produtos);
                    return ERRO_NA_LEITURA_DE_ARQUIVO;
                }
        }
    }

    fclose(arquivo_produtos);
    return SUCESSO_OPERACAO;
}


int fazer_pedido(){

    FILE *arquivo_produtos;
    tp_produto produtos;
    tp_movimentacao pedidos;

    int i = 0, codigo, qtdePedida = 0, status, posicao = 0;

    printf("\n    ==== REALIZAR PEDIDO ====\n");
    if (sizeof(tp_produto) > 0) {

        printf("\nEntre com o codigo do produto: ");
        scanf("%d", &codigo);
        getchar();

        status = encontra_produto(codigo);
        if (status != REGISTRO_EXISTENTE) return status;


        //--------------------------------------------------------------------

        if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "rb+")) == NULL)
        return ERRO_NA_ABERTURA_DE_ARQUIVO;

        if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0){
            if (!feof(arquivo_produtos)){
                fclose(arquivo_produtos);
                return ERRO_NA_LEITURA_DE_ARQUIVO;
            }
        }

        while(!feof(arquivo_produtos)){

            if (produtos.codigo == codigo){

                if (produtos.estoque <= produtos.estoqueMinimo) {
                    printf("\nAtencao! O estoque atual esta abaixo do estoque minimo.");
                    pedidos.contador_est_min++;
                }

                printf("\nEntre com numero do pedido: ");
                scanf("%d", &pedidos.codigo_pedido);
                getchar();
                printf("\nEntre com a quantidade do pedido: ");
                scanf("%d", &pedidos.qtde_pedida);
                getchar();

                if (pedidos.qtde_pedida > produtos.estoque || produtos.estoque == 0) {
                    printf("\nEstoque insuficiente. Nao e possivel realizar o pedido");
                    pedidos.pedido_recusado++;
                } else {
                    if (pedidos.qtde_pedida > pedidos.maior_pedido) {
                        pedidos.maior_pedido = pedidos.qtde_pedida;
                        pedidos.numero_maior = pedidos.codigo_pedido;
                    }
                    produtos.estoque -= pedidos.qtde_pedida;
                    pedidos.soma_pedido += pedidos.qtde_pedida;
                    produtos.pedidos_total++;
                    pedidos.preco_total+= (float)pedidos.qtde_pedida * produtos.preco;
                    printf("Pedido realizado");
                }

                // ftell() obtem a posicao corrente  do  arquivo,  apos  o fread
                // a posicao e o  próximo  registro. Então, para obter a posição
                // correta para  atualizar,  deve-se subtrair a posicao corrente
                // do tamanho da estrutura.
                posicao = ftell(arquivo_produtos) - sizeof(tp_produto);
                fseek(arquivo_produtos, posicao, SEEK_SET);
                if (fwrite(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0)
                    return ERRO_NA_GRAVACAO_DE_ARQUIVO;

                break;
            }

            if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0)
            {
                if (!feof(arquivo_produtos))
                {
                    fclose(arquivo_produtos);
                    return ERRO_NA_LEITURA_DE_ARQUIVO;
                }
            }
        }


        //--------------------------------------------------------------------


    } else {
        printf("\nNao e possivel realizar pedido. Nao ha produtos cadastrados no sistema.");
    }

    fclose(arquivo_produtos);
    return SUCESSO_OPERACAO;
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
    printf("\nBBBB  EEEE M   M     V     V III N   N DDD   OOO\n"
           "B   B E    MM MM     V     V  I  NN  N D  D O   O\n"
           "BBBB  EEE  M M M ---  V   V   I  N N N D  D O   O\n"
           "B   B E    M   M       V V    I  N  NN D  D O   O\n"
           "BBBB  EEEE M   M        V    III N   N DDD   OOO \n\n");
}

void imprime_mensagem_erro_arquivo(int erro, char *nome_arquivo) {
    switch(erro)
    {
    case ERRO_NA_CRIACAO_DE_ARQUIVO:
        printf("\nErro na criacao do arquivo %s!", nome_arquivo);
        break;
    case ERRO_NA_ABERTURA_DE_ARQUIVO:
        printf("\nErro na abertura do arquivo %s!", nome_arquivo);
        break;
    case ERRO_NA_LEITURA_DE_ARQUIVO:
        printf("\nErro na leitura do arquivo %s!",  nome_arquivo);
        break;
    case ERRO_NA_GRAVACAO_DE_ARQUIVO:
        printf("\nErro na gravacao do arquivo %s!",  nome_arquivo);
        break;
    case REGISTRO_EXISTENTE:
        printf("\nRegistro já existente no arquivo %s!",  nome_arquivo);
        break;
    case REGISTRO_NAO_ENCONTRADO:
        printf("\nRegistro não encontrado no arquivo %s!",  nome_arquivo);
        break;
    default:
        printf("\nCódigo de erro desconhecido!");
    }
    return;
}

int cria_arquivo_produtos() {
    FILE *arquivo_produtos;

    if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "rb")) == NULL)
        if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "wb")) == NULL)
           return ERRO_NA_CRIACAO_DE_ARQUIVO;

    fclose(arquivo_produtos);
    return SUCESSO_OPERACAO;

}
