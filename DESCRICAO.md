# Descrição do sistema

## Nome e ideia

- Controle de Gastos é um programa para qualquer pessoa controlar seus gastos por categoria; maiores gastos do mês e as categorias que passaram do limite por exemplo.

## O que o sistema guarda

- Gasto: descrição, valor, data, nome da categoria.
- Categoria: nome e limite mensal.


## O que dá para fazer
 1. Cadastrar categoria: pede o nome e mostra "cadastrada" ou erro.
 2. Definir limite: pede o nome da categoria e o novo limite.
 3. Registrar gastos: pede descrição, valor, data e categoria.
 4. Total por categoria: pede mês e ano, mostra cada categoria com o total do mês e o limite
 5. Maiores gastos do mês: pede mês e ano, mostra os três maiores em ordem decrescente.
 6. Categoria acima do limite: pede mês e ano, lista as que passaram.
 0. Sair.

## Regras

- Valor gasto precisa ser maior que zero.
- Nome da categoria é único.
- Gasto só pode ir para categoria já cadastrada.
- Data digitada como dd/mm/aaaa; dia de 1 a 31, mês de 1 a 12.
- Limite não pode ser negativo.
- Passou do limite: Quando o total do mês for maior que o limite, se for igual a ele ainda está dentro.
- Categoria com limite 0 não entra na lista de estouradas.
- Total por categoria: pede mês e ano e mostra o total daquele mês de cada categoria ao lado do limite mensal.
- O menu sai com a opção 0.
- Data válida: dia 31 em mês de 30 dias e 29/02 em ano não bissexto são inválidos.
- Nomes de categorias valem exatamente como digitamos ("Mercado" e "mercado" são diferentes).
- Empate nos maiores gastos: aparece primeiro o que foi registrado antes.
- Se houver menos de 3 gastos no mês, mostra só os que existem.
- Valor digitado com ponto (10.50).
- A descrição pode ter espaços; a IA escolhe o jeito mais simples de ler.

## Quando algo dá errado

- Categoria já existe: "Categoria já cadastrada".
- Categoria não existe: "Categoria não encontrada".
- Nenhum gasto no mês pedido: "Nenhum gasto registrado nesse mês".
- Nenhuma categoria estourou:"Nenhuma categoria passou do limite".
- Letra onde era número:"Entrada inválida, tente de novo" e volta ao menu.
- Valor menor ou igual a 0 ou data inválida: "Valor inválido" / "Data inválida".
- Opção de menu que não existe: "Opção inválida, tente de novo" e volta ao menu.
- Mês ou ano inválido nas consultas (opções 4, 5 e 6): "Data inválida" e volta ao menu.

## Classes que eu imagino

- Gasto: guarda descrição, valor, data e nome da categoria. Sabe se comparar com outro gasto pelo valor (operador<).
- Categoria: guarda o nome e o limite mensal. Sabe dizer se um total passou do limite.
- Carteira: guarda as categorias e os gastos, aplica as regras (categoria única, valor maior que zero, data válida) e calcula os totais.
- Interface (menu) fica em arquivos separados: lê teclado e escreve na tela, sem regras dentro. Não é uma classe do sistema.

## Exemplo de uso

Escolha: 1
Nome da categoria: Mercado
Categoria cadastrada
Escolha: 2
Categoria: Mercado
Limite: 500
Limite definido
Escolha: 3
Descrição: Compras da semana 
Valor: 120 
Data: 22/10/2026 
Categoria: Mercado
Gasto registrado
Escolha: 4
Mês: 10
Ano: 2026
Mercado: 120.00 de 500.00
Escolha: 0