# Pseudocódigo — Sistema de Vendas

## Estruturas de dados

```
ESTRUTURA Produto
    codigo: inteiro
    nome: texto
    preco: real
FIM ESTRUTURA

ESTRUTURA Cliente
    codigo: inteiro
    nome: texto
FIM ESTRUTURA

ESTRUTURA ItemPedido
    produto: Produto
    quantidade: inteiro
    valorTotalItem: real
FIM ESTRUTURA

ESTRUTURA Pedido
    numero: inteiro
    cliente: Cliente
    itens: array[10] de ItemPedido
    qtdItens: inteiro
    valorTotalPedido: real
    dia, mes: inteiro
FIM ESTRUTURA

ESTRUTURA RegistroDevolucao
    codigoProduto: inteiro
    codigoCliente: inteiro
    qtdDevolucoes: inteiro
FIM ESTRUTURA

CONSTANTE MAX_PEDIDOS_DIA = 50   // restrição: a loja realiza 50 vendas/dia
CONSTANTE TAXA_DEVOLUCAO  = 20.00
```

## Cadastro

```
FUNÇÃO cadastrarProduto(codigo, nome, preco): Produto
    RETORNA um Produto preenchido com os dados informados

FUNÇÃO cadastrarCliente(codigo, nome): Cliente
    RETORNA um Cliente preenchido com os dados informados
```

## Cálculo do valor do pedido

```
FUNÇÃO calcularValorItem(produto, quantidade): real
    RETORNA produto.preco * quantidade

FUNÇÃO calcularValorPedido(pedido)
    total <- 0
    PARA cada item em pedido.itens
        item.valorTotalItem <- calcularValorItem(item.produto, item.quantidade)
        total <- total + item.valorTotalItem
    FIM PARA
    pedido.valorTotalPedido <- total
```

## Regra de negócio: devolução

```
FUNÇÃO registrarDevolucao(historico, codigoProduto, codigoCliente): real
    PARA cada registro em historico
        SE registro.codigoProduto = codigoProduto E
           registro.codigoCliente = codigoCliente ENTÃO
            registro.qtdDevolucoes <- registro.qtdDevolucoes + 1
            SE registro.qtdDevolucoes >= 2 ENTÃO
                RETORNA TAXA_DEVOLUCAO   // R$ 20,00 a partir da 2ª devolução
            FIM SE
            RETORNA 0
        FIM SE
    FIM PARA
    // é a primeira devolução deste produto/cliente
    ADICIONA novo registro em historico com qtdDevolucoes = 1
    RETORNA 0
```

## Relatório diário

```
FUNÇÃO relatorioDiario(pedidos, qtdPedidos): real
    totalDia <- 0
    PARA cada pedido em pedidos
        EXIBE numero, cliente, valorTotalPedido
        totalDia <- totalDia + pedido.valorTotalPedido
    FIM PARA
    EXIBE "Total do dia:", totalDia
    RETORNA totalDia
```

## Relatório mensal

```
FUNÇÃO relatorioMensal(totalPorMes[12]): real
    totalConsolidado <- 0
    PARA i de 0 até 11
        EXIBE nomeMes[i], totalPorMes[i]
        totalConsolidado <- totalConsolidado + totalPorMes[i]
    FIM PARA
    EXIBE "Total consolidado do ano:", totalConsolidado
    RETORNA totalConsolidado
```

## Ordenação (usada no relatório anual) — Bubble Sort decrescente

```
FUNÇÃO ordenarMesesPorFaturamento(meses[12])
    PARA i de 0 até 10
        PARA j de 0 até (10 - i)
            SE meses[j].valorTotal < meses[j+1].valorTotal ENTÃO
                TROCA meses[j] COM meses[j+1]
            FIM SE
        FIM PARA
    FIM PARA
```

## Relatório anual

```
FUNÇÃO relatorioAnual(totalPorMes[12])
    meses <- vetor com (mes, valorTotal) para cada um dos 12 meses
    totalAno <- soma de totalPorMes
    ordenarMesesPorFaturamento(meses)     // ordem decrescente
    EXIBE "Total do ano:", totalAno
    PARA cada mes em meses (já ordenado)
        EXIBE nomeMes, valorTotal
    FIM PARA
```

## Programa principal (visão geral)

```
INÍCIO
    cadastra produtos
    cadastra clientes
    monta array de pedidos do dia (máx. 50)
    para cada pedido: calcularValorPedido()

    testa registrarDevolucao() três vezes para o mesmo produto/cliente
      -> confirma que a taxa só é cobrada a partir da 2ª devolução

    chama relatorioDiario()
    chama relatorioMensal()
    chama relatorioAnual()
FIM
```
