# Sistema de Vendas Interativo 🛒
### Projeto de Extensão II — Bacharelado em Ciência da Computação — UVA (Universidade Veiga de Almeida)

Este repositório contém um **Sistema de Vendas baseado em Terminal** desenvolvido em **Linguagem C**. O projeto atende integralmente aos requisitos práticos exigidos nas diretrizes acadêmicas do Projeto de Extensão II da UVA, utilizando estruturas de dados clássicas, modularização rigorosa e uma lógica de negócio interativa para simular a operação diária de uma microempresa comercial.

Ao contrário de abordagens estáticas, esta versão é **100% interativa**: o usuário realiza o cadastro dinâmico de produtos, clientes, movimentações de caixa (vendas) e logística reversa (devoluções) diretamente no console.

---

## 🚀 Funcionalidades Principais

*   **Módulo de Cadastros Dinâmicos:**
    *   Cadastro de produtos com geração automatizada de código único e definição de preço unitário.
    *   Cadastro de clientes com identificação automática sequencial.
*   **Módulo de Vendas (Pedidos):**
    *   Abertura de ordens de venda vinculando um cliente registrado a múltiplos produtos.
    *   Cálculo automático de subtotais por item e do valor consolidado do pedido.
    *   Atribuição de data (dia/mês) para composição histórica dos relatórios.
*   **Regra de Negócio Crítica — Logística Reversa (Devoluções):**
    *   Rastreamento do histórico de interações entre clientes e produtos devolvidos.
    *   **Política de Cobrança:** A primeira devolução de um produto específico feita por um cliente não gera ônus. A partir da **segunda devolução do mesmo produto pelo mesmo cliente**, o sistema aplica automaticamente uma taxa de transporte fixa de **R$ 20,00**.
*   **Módulo de Inteligência Comercial e Relatórios:**
    *   **Relatório Diário:** Consolidação e listagem detalhada de todas as vendas registradas no dia operacional vigente.
    *   **Relatório Mensal:** Apuração matemática do faturamento segmentado por cada um dos 12 meses do ano.
    *   **Relatório Anual com Classificação:** Apresentação do faturamento bruto anual total, acompanhado do ranqueamento dos meses em **ordem decrescente de receita** através de ordenação algorítmica.

---

## 🎓 Requisitos Técnicos e Acadêmicos Atendidos

O código foi projetado para demonstrar proficiência em fundamentos de desenvolvimento de software e estruturas de dados estáticas em C:

1.  **Abstração por Estruturas (`struct`):**
    *   Modelagem robusta de entidades através de estruturas como `Produto`, `Cliente`, `ItemPedido`, `Pedido`, `RegistroDevolucao` e `FaturamentoMes`.
2.  **Agrupamentos em Memória (`arrays`):**
    *   Uso estratégico de vetores de tamanho fixo definidos por diretivas de compilação (`#define`), garantindo previsibilidade e estabilidade de memória.
3.  **Algoritmo de Ordenação Decrescente (Bubble Sort):**
    *   Implementação manual do algoritmo de ordenação por bolha na função `ordenarMesesPorFaturamento()`, evidenciando a capacidade de manipulação e classificação de dados brutos sem o auxílio de bibliotecas externas de alto nível.
4.  **Modularização e Funções de Responsabilidade Única:**
    *   Estrutura limpa de sub-rotinas onde cada funcionalidade (cadastros, listagens, regras fiscais e relatórios) opera de forma isolada e previsível.
5.  **Robustez na Captura de Dados (Sanitização de Input):**
    *   Desenvolvimento de rotinas auxiliares (`limparBufferEntrada()`, `lerInteiro()`, `lerFloat()`, `lerTexto()`) projetadas para evitar o clássico travamento por estouro de buffer do teclado ou loops infinitos decorrentes de entradas do usuário inválidas.

---

## 🛠️ Como Executar o Projeto

### Pré-requisitos
Para compilar e rodar este projeto, você precisa apenas de um compilador de Linguagem C instalado no seu ambiente de desenvolvimento (como o `GCC`, `Clang` ou MSVC).

### Passo a Passo

1.  **Clonar ou Salvar o Código:**
    Faça o download ou copie o código fonte e salve-o em um arquivo chamado `main.c`.

2.  **Compilar via Terminal:**
    Abra o seu terminal/prompt de comando na pasta do arquivo e execute a diretiva do GCC:
    ```bash
    gcc main.c -o sistema_vendas
    ```

3.  **Executar a Aplicação:**
    *   **Linux / macOS:**
        ```bash
        ./sistema_vendas
        ```
    *   **Windows:**
        ```bash
        sistema_vendas.exe
        ```

---

## 📊 Configurações e Limites do Sistema

O comportamento estático do software é regido por limites bem definidos em suas diretivas em tempo de compilação. Essas métricas podem ser reajustadas alterando o arquivo-fonte diretamente:

| Diretiva | Valor Limite | Escopo e Aplicação |
| :--- | :---: | :--- |
| `MAX_PRODUTOS` | `100` | Limite de produtos cadastrados no catálogo em memória. |
| `MAX_CLIENTES` | `100` | Capacidade máxima de armazenamento do banco de clientes. |
| `MAX_PEDIDOS_DIA` | `50` | Volume máximo de cupons fiscais operados na seção diária. |
| `MAX_ITENS_PEDIDO` | `10` | Quantidade máxima de produtos diferentes aceitos por carrinho de compra. |
| `MAX_HISTORICO` | `100` | Armazenamento de registros de auditoria para aplicação da taxa de devolução. |
| `TAXA_DEVOLUCAO` | `R$ 20.00` | Encargo de logística cobrado na recorrência de devoluções. |

---

## 📂 Arquitetura das Estruturas de Dados (`structs`)

Para melhor compreensão acadêmica da modelagem relacional feita em memória RAM, segue a organização lógica das estruturas empregadas no código:

*   `Produto`: Armazena o código (`int`), o nome descritivo (`char[]`) e o preço de venda (`float`).
*   `Cliente`: Armazena o código único (`int`) e o nome completo (`char[]`).
*   `ItemPedido`: Representa o carrinho, acoplando a estrutura `Produto`, a quantidade solicitada (`int`) e o subtotal calculado (`float`).
*   `Pedido`: Reúne o número sequencial da venda, a estrutura `Cliente` associada, o array de `ItemPedido`, a data (dia/mês) e o total líquido da transação.
*   `RegistroDevolucao`: Mapeia e incrementa a quantidade de ocorrências (`qtdDevolucoes`) cruzando o `codigoProduto` com o `codigoCliente`.

---

> ⚠️ **Nota Importante sobre Persistência:**
> Como este sistema foi estruturado com foco estrito em lógica de ponteiros, passagem de parâmetros por referência e manipulação de arrays estáticos em memória, **os dados coletados residem exclusivamente na memória RAM**. Ao encerrar a execução do programa (Opção `0`), todos os registros inseridos manualmente serão descartados. 

---
Desenvolvido como critério de avaliação prática para o **Projeto de Extensão II - UVA**.
