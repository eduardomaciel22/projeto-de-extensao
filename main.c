

#include <stdio.h>
#include <string.h>

#define MAX_PRODUTOS      100
#define MAX_CLIENTES      100
#define MAX_PEDIDOS_DIA    50   /* a loja realiza 50 vendas por dia */
#define MAX_ITENS_PEDIDO   10
#define MAX_HISTORICO     100
#define TAXA_DEVOLUCAO    20.00f
#define MESES_ANO          12



typedef struct {
    int   codigo;
    char  nome[50];
    float preco;
} Produto;

typedef struct {
    int  codigo;
    char nome[50];
} Cliente;

typedef struct {
    Produto produto;
    int     quantidade;
    float   valorTotalItem;
} ItemPedido;

typedef struct {
    int        numero;
    Cliente    cliente;
    ItemPedido itens[MAX_ITENS_PEDIDO];
    int        qtdItens;
    float      valorTotalPedido;
    int        dia;
    int        mes;
} Pedido;

/* guarda quantas vezes um determinado produto/cliente já foi devolvido */
typedef struct {
    int codigoProduto;
    int codigoCliente;
    int qtdDevolucoes;
} RegistroDevolucao;

typedef struct {
    int   mes;
    float valorTotal;
} FaturamentoMes;



void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int lerInteiro(const char *mensagem) {
    int valor;
    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1) {
        printf("Valor invalido. %s", mensagem);
        limparBufferEntrada();
    }
    limparBufferEntrada();
    return valor;
}

float lerFloat(const char *mensagem) {
    float valor;
    printf("%s", mensagem);
    while (scanf("%f", &valor) != 1) {
        printf("Valor invalido. %s", mensagem);
        limparBufferEntrada();
    }
    limparBufferEntrada();
    return valor;
}

void lerTexto(const char *mensagem, char *destino, int tamanho) {
    printf("%s", mensagem);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0'; /* remove o \n do final */
}

/* ---------------------- CADASTROS (MANUAIS / INTERATIVOS) ---------------------- */

void cadastrarProdutoInterativo(Produto produtos[], int *qtdProdutos) {
    if (*qtdProdutos >= MAX_PRODUTOS) {
        printf("Limite maximo de produtos atingido.\n");
        return;
    }
    Produto p;
    p.codigo = *qtdProdutos + 1; /* codigo gerado automaticamente */
    lerTexto("Nome do produto: ", p.nome, sizeof(p.nome));
    p.preco = lerFloat("Preco do produto (R$): ");

    produtos[*qtdProdutos] = p;
    (*qtdProdutos)++;
    printf(">> Produto \"%s\" cadastrado com o codigo %d.\n\n", p.nome, p.codigo);
}

void cadastrarClienteInterativo(Cliente clientes[], int *qtdClientes) {
    if (*qtdClientes >= MAX_CLIENTES) {
        printf("Limite maximo de clientes atingido.\n");
        return;
    }
    Cliente c;
    c.codigo = *qtdClientes + 1; /* codigo gerado automaticamente */
    lerTexto("Nome do cliente: ", c.nome, sizeof(c.nome));

    clientes[*qtdClientes] = c;
    (*qtdClientes)++;
    printf(">> Cliente \"%s\" cadastrado com o codigo %d.\n\n", c.nome, c.codigo);
}

void listarProdutos(Produto produtos[], int qtdProdutos) {
    int i;
    if (qtdProdutos == 0) {
        printf("Nenhum produto cadastrado ainda.\n");
        return;
    }
    printf("\n--- Produtos cadastrados ---\n");
    for (i = 0; i < qtdProdutos; i++) {
        printf("Codigo %d | %-20s | R$ %.2f\n",
               produtos[i].codigo, produtos[i].nome, produtos[i].preco);
    }
    printf("\n");
}

void listarClientes(Cliente clientes[], int qtdClientes) {
    int i;
    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado ainda.\n");
        return;
    }
    printf("\n--- Clientes cadastrados ---\n");
    for (i = 0; i < qtdClientes; i++) {
        printf("Codigo %d | %s\n", clientes[i].codigo, clientes[i].nome);
    }
    printf("\n");
}

int buscarProdutoPorCodigo(Produto produtos[], int qtdProdutos, int codigo) {
    int i;
    for (i = 0; i < qtdProdutos; i++) {
        if (produtos[i].codigo == codigo) return i;
    }
    return -1;
}

int buscarClientePorCodigo(Cliente clientes[], int qtdClientes, int codigo) {
    int i;
    for (i = 0; i < qtdClientes; i++) {
        if (clientes[i].codigo == codigo) return i;
    }
    return -1;
}

/* ---------------------- CÁLCULO DE VALORES ---------------------- */

float calcularValorItem(Produto produto, int quantidade) {
    return produto.preco * quantidade;
}

void calcularValorPedido(Pedido *pedido) {
    float total = 0.0f;
    int i;
    for (i = 0; i < pedido->qtdItens; i++) {
        pedido->itens[i].valorTotalItem =
            calcularValorItem(pedido->itens[i].produto, pedido->itens[i].quantidade);
        total += pedido->itens[i].valorTotalItem;
    }
    pedido->valorTotalPedido = total;
}

/* ---------------------- CRIACAO DE PEDIDO (MANUAL) ---------------------- */

void criarPedidoInterativo(Pedido pedidosDoDia[], int *qtdPedidos,
                            Produto produtos[], int qtdProdutos,
                            Cliente clientes[], int qtdClientes,
                            float totalPorMes[MESES_ANO]) {

    if (*qtdPedidos >= MAX_PEDIDOS_DIA) {
        printf("A loja ja atingiu o limite de %d vendas no dia.\n", MAX_PEDIDOS_DIA);
        return;
    }
    if (qtdProdutos == 0 || qtdClientes == 0) {
        printf("Cadastre pelo menos um produto e um cliente antes de criar um pedido.\n");
        return;
    }

    listarClientes(clientes, qtdClientes);
    int codCliente = lerInteiro("Informe o codigo do cliente: ");
    int idxCliente = buscarClientePorCodigo(clientes, qtdClientes, codCliente);
    if (idxCliente == -1) {
        printf("Cliente nao encontrado.\n");
        return;
    }

    Pedido pedido;
    pedido.numero = *qtdPedidos + 1;
    pedido.cliente = clientes[idxCliente];
    pedido.dia = lerInteiro("Dia da venda (1-31): ");
    pedido.mes = lerInteiro("Mes da venda (1-12): ") - 1; /* indice 0-11 */

    listarProdutos(produtos, qtdProdutos);
    int qtdItens = lerInteiro("Quantos produtos diferentes tem nesse pedido? ");
    if (qtdItens > MAX_ITENS_PEDIDO) qtdItens = MAX_ITENS_PEDIDO;
    pedido.qtdItens = 0;

    int i;
    for (i = 0; i < qtdItens; i++) {
        int codProduto = lerInteiro("  Codigo do produto: ");
        int idxProduto = buscarProdutoPorCodigo(produtos, qtdProdutos, codProduto);
        if (idxProduto == -1) {
            printf("  Produto nao encontrado, item ignorado.\n");
            continue;
        }
        int quantidade = lerInteiro("  Quantidade: ");
        pedido.itens[pedido.qtdItens].produto = produtos[idxProduto];
        pedido.itens[pedido.qtdItens].quantidade = quantidade;
        pedido.qtdItens++;
    }

    calcularValorPedido(&pedido);
    pedidosDoDia[*qtdPedidos] = pedido;
    (*qtdPedidos)++;

    if (pedido.mes >= 0 && pedido.mes < MESES_ANO) {
        totalPorMes[pedido.mes] += pedido.valorTotalPedido;
    }

    printf(">> Pedido %d cadastrado para %s. Valor total: R$ %.2f\n\n",
           pedido.numero, pedido.cliente.nome, pedido.valorTotalPedido);
}

/* ---------------------- REGRA DE NEGÓCIO: DEVOLUÇÃO ---------------------- */
/* Caso um item seja devolvido pela SEGUNDA vez (mesmo produto + mesmo
   cliente), cobra-se uma taxa fixa de R$ 20,00 referente à transportadora. */

float registrarDevolucao(RegistroDevolucao historico[], int *qtdHistorico,
                          int codigoProduto, int codigoCliente) {
    int i;
    for (i = 0; i < *qtdHistorico; i++) {
        if (historico[i].codigoProduto == codigoProduto &&
            historico[i].codigoCliente == codigoCliente) {
            historico[i].qtdDevolucoes++;
            if (historico[i].qtdDevolucoes >= 2) {
                return TAXA_DEVOLUCAO; /* cobra a taxa a partir da 2a devolução */
            }
            return 0.0f;
        }
    }
    /* primeira devolução deste produto/cliente: cria registro novo */
    historico[*qtdHistorico].codigoProduto  = codigoProduto;
    historico[*qtdHistorico].codigoCliente  = codigoCliente;
    historico[*qtdHistorico].qtdDevolucoes  = 1;
    (*qtdHistorico)++;
    return 0.0f;
}

void registrarDevolucaoInterativo(RegistroDevolucao historico[], int *qtdHistorico,
                                   Produto produtos[], int qtdProdutos,
                                   Cliente clientes[], int qtdClientes) {
    if (qtdProdutos == 0 || qtdClientes == 0) {
        printf("Cadastre pelo menos um produto e um cliente antes de registrar devolucao.\n");
        return;
    }
    listarProdutos(produtos, qtdProdutos);
    int codProduto = lerInteiro("Codigo do produto devolvido: ");
    listarClientes(clientes, qtdClientes);
    int codCliente = lerInteiro("Codigo do cliente que devolveu: ");

    float taxa = registrarDevolucao(historico, qtdHistorico, codProduto, codCliente);
    if (taxa > 0) {
        printf(">> Esta e a 2a (ou mais) devolucao deste produto por este cliente. "
               "Taxa cobrada: R$ %.2f\n\n", taxa);
    } else {
        printf(">> Devolucao registrada. Nenhuma taxa cobrada nesta devolucao.\n\n");
    }
}

/* ---------------------- RELATÓRIO DIÁRIO ---------------------- */

float relatorioDiario(Pedido pedidos[], int qtdPedidos) {
    float totalDia = 0.0f;
    int i;
    printf("\n===================== RELATORIO DIARIO =====================\n");
    if (qtdPedidos == 0) {
        printf("Nenhum pedido registrado ainda.\n");
    }
    for (i = 0; i < qtdPedidos; i++) {
        printf("Pedido %d | Cliente: %-20s | Dia %02d | Valor: R$ %.2f\n",
               pedidos[i].numero, pedidos[i].cliente.nome,
               pedidos[i].dia, pedidos[i].valorTotalPedido);
        totalDia += pedidos[i].valorTotalPedido;
    }
    printf("--------------------------------------------------------------\n");
    printf("VALOR TOTAL DAS VENDAS DO DIA: R$ %.2f\n", totalDia);
    printf("================================================================\n\n");
    return totalDia;
}

/* ---------------------- RELATÓRIO MENSAL ---------------------- */

float relatorioMensal(float totalPorMes[MESES_ANO]) {
    const char *nomesMeses[MESES_ANO] = {
        "Janeiro","Fevereiro","Marco","Abril","Maio","Junho",
        "Julho","Agosto","Setembro","Outubro","Novembro","Dezembro"
    };
    float totalConsolidado = 0.0f;
    int i;
    printf("\n===================== RELATORIO MENSAL =====================\n");
    for (i = 0; i < MESES_ANO; i++) {
        printf("%-10s: R$ %.2f\n", nomesMeses[i], totalPorMes[i]);
        totalConsolidado += totalPorMes[i];
    }
    printf("--------------------------------------------------------------\n");
    printf("VALOR TOTAL CONSOLIDADO DAS VENDAS MENSAIS: R$ %.2f\n", totalConsolidado);
    printf("================================================================\n\n");
    return totalConsolidado;
}

/* ---------------------- ORDENAÇÃO (BUBBLE SORT) ---------------------- */
/* Ordena os meses em ordem DECRESCENTE de faturamento */

void ordenarMesesPorFaturamento(FaturamentoMes meses[], int n) {
    int i, j;
    FaturamentoMes aux;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (meses[j].valorTotal < meses[j + 1].valorTotal) {
                aux = meses[j];
                meses[j] = meses[j + 1];
                meses[j + 1] = aux;
            }
        }
    }
}

/* ---------------------- RELATÓRIO ANUAL ---------------------- */

void relatorioAnual(float totalPorMes[MESES_ANO]) {
    const char *nomesMeses[MESES_ANO] = {
        "Janeiro","Fevereiro","Marco","Abril","Maio","Junho",
        "Julho","Agosto","Setembro","Outubro","Novembro","Dezembro"
    };
    FaturamentoMes meses[MESES_ANO];
    float totalAno = 0.0f;
    int i;

    for (i = 0; i < MESES_ANO; i++) {
        meses[i].mes = i;
        meses[i].valorTotal = totalPorMes[i];
        totalAno += totalPorMes[i];
    }

    ordenarMesesPorFaturamento(meses, MESES_ANO);

    printf("\n===================== RELATORIO ANUAL =====================\n");
    printf("VALOR TOTAL DE VENDAS NO ANO: R$ %.2f\n\n", totalAno);
    printf("Meses em ordem decrescente de faturamento:\n");
    for (i = 0; i < MESES_ANO; i++) {
        printf("%2d. %-10s - R$ %.2f\n", i + 1,
               nomesMeses[meses[i].mes], meses[i].valorTotal);
    }
    printf("================================================================\n\n");
}

/* ---------------------- MENU PRINCIPAL ---------------------- */

void exibirMenu(void) {
    printf("=================== SISTEMA DE VENDAS ===================\n");
    printf("1 - Cadastrar produto\n");
    printf("2 - Cadastrar cliente\n");
    printf("3 - Listar produtos e clientes cadastrados\n");
    printf("4 - Registrar novo pedido (venda)\n");
    printf("5 - Registrar devolucao de item\n");
    printf("6 - Gerar relatorio diario\n");
    printf("7 - Gerar relatorio mensal\n");
    printf("8 - Gerar relatorio anual\n");
    printf("0 - Sair\n");
    printf("===========================================================\n");
}

int main(void) {

    Produto produtos[MAX_PRODUTOS];
    int qtdProdutos = 0;

    Cliente clientes[MAX_CLIENTES];
    int qtdClientes = 0;

    Pedido pedidosDoDia[MAX_PEDIDOS_DIA];
    int qtdPedidosHoje = 0;

    RegistroDevolucao historicoDevolucoes[MAX_HISTORICO];
    int qtdHistorico = 0;

    float totalPorMes[MESES_ANO] = {0};

    int opcao;

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ");
        printf("\n");

        switch (opcao) {
            case 1:
                cadastrarProdutoInterativo(produtos, &qtdProdutos);
                break;
            case 2:
                cadastrarClienteInterativo(clientes, &qtdClientes);
                break;
            case 3:
                listarProdutos(produtos, qtdProdutos);
                listarClientes(clientes, qtdClientes);
                break;
            case 4:
                criarPedidoInterativo(pedidosDoDia, &qtdPedidosHoje,
                                       produtos, qtdProdutos,
                                       clientes, qtdClientes,
                                       totalPorMes);
                break;
            case 5:
                registrarDevolucaoInterativo(historicoDevolucoes, &qtdHistorico,
                                             produtos, qtdProdutos,
                                             clientes, qtdClientes);
                break;
            case 6:
                relatorioDiario(pedidosDoDia, qtdPedidosHoje);
                break;
            case 7:
                relatorioMensal(totalPorMes);
                break;
            case 8:
                relatorioAnual(totalPorMes);
                break;
            case 0:
                printf("Encerrando o sistema. Ate mais!\n");
                break;
            default:
                printf("Opcao invalida, tente novamente.\n\n");
        }

    } while (opcao != 0);

    return 0;
}
1