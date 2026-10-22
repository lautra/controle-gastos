# Descrição do sistema

Escreva este arquivo sem IA, antes de qualquer código. Apague as instruções
em itálico quando preencher.

## Nome e ideia

_Duas ou três frases: o que o sistema é e para quem serve.
- Controle de gastos é um programa para qualquer pessoa controlar seus gastos por categoria; maiores gastos do mês e as categorias que passaram do limite por exemplo.

## O que o sistema guarda

- Gasto: descrição, valor, data, nome da categoria
- Categoria: nome e limite mensal


## O que dá para fazer
 1. Cadastrar categoria: pede o nome e mostra "cadastrada' ou erro
 2. Definir limite: pede o nome da categoria e o novo limite
 3. Registrar gastos: pede descrição, valor, data e categoria
 4. Total por categoria: mostra cada categoria como o total gasto e o limite
 5. Maiores gastos mensal: pede mês e ano, mostra os três maiores em ordem decrescente
 6. Categoria acima do limite: pede mês e ano, lista as que passaram
 7. Sair



## Regras

- Valor gasto precisa ser maior que zero
- Nome da categoria é único
- GAto só pode ir para categoria já cadastrada
- Data digitada como dd/mm/aaaa; dia de 1 a 31, mês de 1 a 12
- Limite não pode ser negativo
- PAssou do limite: Quando o total do mês for maior que o limite, se for igual a ele ainda está dentro
- Categoria com limite 0 não entra na lista de estouradas

## Quando algo dá errado

- Categoria já existe: "CAtegoria já cadastrada"
- Categoria não existe: "CAtegoria não encontrada"
- Nenhum gasto no mês pedido: "Nenhum gasto resgistrado nesse mês"
- Nenhuma categoria estourou:"Nenhuma categoria passou do limite"
- Letra onde era número:"Entrada inválida, tente de novo" e volta ao menu
- Valor menor ou igual a 0 ou data inválida: "VAlor inválido" / "DAta inválida"

## Classes que eu imagino

- GAsto: guarda descrição, valor, data e categoria, e sabe  se comparar com outro pelo valor (operador<)
- CAtegoria: guarda todas as categoriass e gastos, aplica as regras e calcula totais
-Interface (menu): lê teclado e escreve na tela, sem regras dentro

## Exemplo de uso

EScolha: 1
Nome da categoria: Mercado
Categoria cadastrada
CAtegoria: Mercado
Limite: 500
Escolha: 3
DEscrição: Compras da semana / VAlor: 120 / DAta: 22/10/2026 / CAtegoria: Mercado
GAsto registrado
EScolha: 4
MErcado: 120,00 de 500,00