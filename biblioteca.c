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
    tp_movimentacao pedidos;

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

        produtos.entradas_total = 0;
        produtos.pedidos_total = 0;
        produtos.pedido_maximo = 0;
        produtos.numero_pedido = 0;
        produtos.qtde_pedida = 0;
        produtos.qtde_entrada = 0;
        produtos.soma_entrada = 0;
        produtos.codigo_pedido = 0;
        produtos.codigo_entrada = 0;
        produtos.soma_pedido = 0;
        produtos.preco_total = 0;
        produtos.maior_pedido = 0;
        produtos.menor_entrada = 0;
        produtos.numero_maior = 0;
        produtos.media_pedidos = 0;
        produtos.media_entradas = 0;
        produtos.contador_est_min = 0;
        produtos.pedido_recusado = 0;
        produtos.entrada_recusada = 0;
        produtos.valor_total_entradas = 0;
        produtos.pedidos_total = 0;
        produtos.entradas_total = 0;


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
    tp_movimentacao pedidos;

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

    int codigo = 0, status, posicao = 0;

    printf("\n    ==== REALIZAR PEDIDO ====\n");
    if (sizeof(tp_produto) > 0) {

        printf("\nEntre com o codigo do produto: ");
        scanf("%d", &codigo);
        getchar();

        status = encontra_produto(codigo);
        if (status != REGISTRO_EXISTENTE) return status;

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
                }

                printf("\nEntre com numero do pedido: ");
                scanf("%d", &produtos.codigo_pedido);
                getchar();
                printf("\nEntre com a quantidade do pedido: ");
                scanf("%d", &produtos.qtde_pedida);
                getchar();

                if (produtos.qtde_pedida > produtos.estoque) {
                    printf("\nEstoque insuficiente. Nao e possivel realizar o pedido");
                    produtos.pedido_recusado++;
                } else {
                    if (produtos.qtde_pedida > produtos.maior_pedido) {
                        produtos.maior_pedido = produtos.qtde_pedida;
                        produtos.numero_maior = produtos.codigo_pedido;
                    }
                    produtos.estoque -= produtos.qtde_pedida;
                    produtos.soma_pedido += produtos.qtde_pedida;
                    produtos.pedidos_total++;
                    produtos.preco_total+= (float)produtos.qtde_pedida * produtos.preco;
                    printf("Pedido realizado");
                    if (produtos.estoque <= produtos.estoqueMinimo) {
                        produtos.contador_est_min++;
                    }
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

    } else {
        printf("\nNao e possivel realizar pedido. Nao ha produtos cadastrados no sistema.");
    }

    fclose(arquivo_produtos);
    return SUCESSO_OPERACAO;
}

int entrada(){
    FILE *arquivo_produtos;
    tp_produto produtos;
    tp_movimentacao pedidos;

    int codigo = 0, status, posicao = 0;

    if (sizeof(tp_produto) > 0) {

        printf("\n    ==== REALIZAR ENTRADA ====\n");
        printf("\nEntre com o codigo do produto: ");
        scanf("%d", &codigo);
        getchar();

        status = encontra_produto(codigo);
        if (status != REGISTRO_EXISTENTE) return status;

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

                if (produtos.estoque >= produtos.estoqueMinimo) {
                    printf("\nA quantidade em estoque e maior ou igual ao estoque minimo. Entrada recusada!");
                    produtos.entrada_recusada++;
                } else {
                    printf("\nEntre com a quantidade de entrada: ");
                    scanf("%d", &produtos.qtde_entrada);
                    getchar();

                    if (produtos.entradas_total == 0) {
                        produtos.menor_entrada = produtos.qtde_entrada;
                    }
                    if (produtos.qtde_entrada < produtos.menor_entrada) {
                        produtos.menor_entrada = produtos.qtde_entrada;
                    }
                    produtos.estoque += produtos.qtde_entrada;
                    produtos.valor_total_entradas += (float)produtos.qtde_entrada * produtos.preco;
                    produtos.soma_entrada+=produtos.qtde_entrada;
                    produtos.entradas_total++;
                    printf("Entrada realizada");
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
    } else {
        printf("\nNao e possivel cadastrar uma entrada. Nao ha produtos cadastrados.");
    }

    fclose(arquivo_produtos);
    return SUCESSO_OPERACAO;
}

int infos_pedidos() {

    FILE *arquivo_produtos;
    tp_produto produtos;

    if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "rb")) == NULL){
        return ERRO_NA_ABERTURA_DE_ARQUIVO;
    }

    printf("\n\n\n\t\t\t\t   ==== INFORMACOES ESTATISTICAS DE PEDIDOS ====\n");

    if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0) {
        if (!feof(arquivo_produtos)) {
            fclose(arquivo_produtos);
            return ERRO_NA_LEITURA_DE_ARQUIVO;
        }
    }

    while(!feof(arquivo_produtos)) {

        if (produtos.pedidos_total == 0) {
            produtos.media_pedidos = 0;
        } else {
            produtos.media_pedidos = (float)produtos.soma_pedido/(float)produtos.pedidos_total;
        }

        printf("\n\n\t\t\t\t\t--- INFORMACOES DO PRODUTO: %s ---", produtos.nome);
        printf("\nQtde Pedidos |\t Preco Total Pedidos |\t Maior Pedido |\tMedia Pedidos |\t Abaixo do Est. Minimo | Pedidos Recusados");
        printf("\n %d\t\t %.2f\t\t\t Nº %d - %d\t\t %.1f\t\t %d\t\t\t\t %d",
               produtos.pedidos_total,
               produtos.preco_total,
               produtos.numero_maior,
               produtos.maior_pedido,
               produtos.media_pedidos,
               produtos.contador_est_min,
               produtos.pedido_recusado);

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

int infos_entradas() {

    FILE *arquivo_produtos;
    tp_produto produtos;

    if((arquivo_produtos = fopen(NOME_ARQUIVO_PRODUTOS, "rb")) == NULL){
        return ERRO_NA_ABERTURA_DE_ARQUIVO;
    }
    printf("\n    ==== INFORMACOES ESTATISTICAS DE ENTRADAS ====\n");

    if (fread(&produtos, sizeof(tp_produto), 1, arquivo_produtos) == 0) {
        if (!feof(arquivo_produtos)) {
            fclose(arquivo_produtos);
            return ERRO_NA_LEITURA_DE_ARQUIVO;
        }
    }

    while(!feof(arquivo_produtos)) {

        if (produtos.entradas_total == 0) {
            produtos.media_entradas = 0;
        } else {
            produtos.media_entradas = (float)produtos.soma_pedido/(float)produtos.pedidos_total;
        }

        printf("\n\n\t\t\t\t\t--- INFORMACOES DO PRODUTO: %s ---", produtos.nome);
        printf("\nQtde Entradas |\t Preco Total Entradas |\t Menor Entrada | Media Entradas | Entradas Recusadas");
        printf("\n %d\t\t %.2f\t\t\t %d\t\t %.1f\t\t\t %d",
               produtos.entradas_total,
               produtos.valor_total_entradas,
               produtos.menor_entrada,
               produtos.media_entradas,
               produtos.entrada_recusada);
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

int menu() {
    int opcao;
    printf("\n\n------------------------------------"
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
