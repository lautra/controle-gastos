# Plano de implementação — Controle de Gastos

## 1. Visão geral

O sistema terá as três classes pedidas na descrição (`Categoria`, `Gasto` e
`Carteira`) e uma interface de menu separada. A `Carteira` guarda os dados e
aplica as regras; a interface apenas lê do teclado, chama a `Carteira` e
mostra as mensagens.

As classes do sistema não usam `cin` nem `cout`. Elas recebem os dados por
parâmetro e devolvem um resultado (um `bool`, um enum ou um container com o
que foi pedido). O `main` só cria a `Carteira` e chama a interface.

## 2. Classes

### Categoria

Arquivos: `include/Categoria.hpp` e `src/Categoria.cpp`.

Guarda:

- `nome` (`std::string`);
- `limite` mensal (`double`).

Métodos:

- construtor que recebe o nome e o limite (limite inicial 0 quando a
  categoria é cadastrada);
- `getNome()`;
- `getLimite()`;
- `setLimite(double)`, chamado pela `Carteira` somente depois de validar;
- `passouDoLimite(double total)`: devolve `true` apenas quando
  `limite > 0` e `total > limite`. Assim, limite 0 nunca entra na lista de
  estouradas e total igual ao limite ainda está dentro.

Não usa container.

### Gasto

Arquivos: `include/Gasto.hpp` e `src/Gasto.cpp`.

Guarda:

- `descricao` (`std::string`);
- `valor` (`double`);
- `data` (`std::string` no formato `dd/mm/aaaa`);
- `categoria` (nome da categoria, `std::string`);
- `ordem` (`int`), um número interno de cadastro usado só para desempatar.

Métodos:

- construtor com descrição, valor, data, categoria e ordem;
- getters para os cinco dados;
- `operator<` comparando **somente pelo valor**, como está na descrição.

A data fica como texto para não criar uma quarta classe sem necessidade. A
validação e a separação em dia/mês/ano ficam na `Carteira`, que é quem aplica
as regras.

### Carteira

Arquivos: `include/Carteira.hpp` e `src/Carteira.cpp`.

Guarda:

- `std::map<std::string, Categoria> categorias`: o nome é a chave. Isso já
  garante nome único e permite procurar uma categoria rapidamente;
- `std::list<Gasto> gastos`: guarda na ordem em que foram cadastrados;
- `int proximaOrdem`: contador usado para preencher a ordem de cada gasto.

Métodos públicos:

- `cadastrarCategoria(nome)`: cadastra com limite inicial 0 ou informa que a
  categoria já existe;
- `definirLimite(nome, limite)`: procura a categoria, recusa limite negativo
  e altera o limite;
- `registrarGasto(descricao, valor, data, categoria)`: valida valor maior
  que zero, data válida e categoria existente; se estiver tudo certo, cria o
  `Gasto` e guarda na lista;
- `mesAnoValido(mes, ano)`: usado pelas consultas 4, 5 e 6;
- `temGastosNoMes(mes, ano)`;
- `totaisPorCategoria(mes, ano)`: devolve um `std::vector<ResumoCategoria>`
  com todas as categorias e o total de cada uma no mês;
- `maioresGastosDoMes(mes, ano)`: devolve até três gastos em ordem
  decrescente de valor;
- `categoriasAcimaDoLimite(mes, ano)`: devolve um
  `std::vector<ResumoCategoria>` só com as categorias que passaram do
  limite.

Resultados de erro: a `Carteira` não devolve texto. Ela devolve enums
simples, por exemplo:

- cadastro: `CATEGORIA_OK` ou `CATEGORIA_JA_EXISTE`;
- limite: `LIMITE_OK`, `LIMITE_CATEGORIA_NAO_ENCONTRADA` ou
  `LIMITE_INVALIDO`;
- gasto: `GASTO_OK`, `GASTO_CATEGORIA_NAO_ENCONTRADA`,
  `GASTO_VALOR_INVALIDO` ou `GASTO_DATA_INVALIDA`.

A interface traduz cada enum na mensagem correspondente da descrição.

Métodos privados:

- `somarGastosDoMes(mes, ano)`: devolve um
  `std::map<std::string, double>` com o total de cada categoria no mês;
- `dataValida(data)`, `separarData(data, dia, mes, ano)`, `bissexto(ano)` e
  `diasNoMes(mes, ano)`: usados na validação e nos filtros por mês/ano.

### Interface

Arquivos: `include/Interface.hpp`, `src/Interface.cpp` e `src/main.cpp`.

- `executarInterface(Carteira &carteira)` fica em `Interface.cpp`;
- `main` apenas cria a `Carteira`, chama `executarInterface` e termina;
- toda leitura e escrita na tela fica nesses arquivos;
- números são lidos com `std::cin >>`; textos (descrição e nomes) são lidos
  com `std::getline`, para permitir espaços;
- quando a leitura de um número falha, a interface limpa o erro, descarta o
  resto da linha e mostra `Entrada inválida, tente de novo`;
- valores em reais aparecem com duas casas decimais
  (`std::fixed` e `std::setprecision(2)`).

### Struct auxiliar ResumoCategoria

Fica declarada em `Carteira.hpp`, sem `.cpp` próprio. Guarda `nome`, `total`
e `limite`. Serve só para a `Carteira` devolver os dados prontos para a
interface, sem que a interface precise acessar o `map` interno.

## 3. Containers e algoritmos da STL

| Onde | O quê | Por quê |
| --- | --- | --- |
| `Carteira::categorias` | `std::map<std::string, Categoria>` | O nome da categoria é único e é a chave. O `map` impede repetição, acha a categoria por nome e ainda percorre em ordem alfabética. Um `vector` exigiria procurar e conferir duplicatas manualmente. |
| `Carteira::gastos` | `std::list<Gasto>` | Guarda os gastos na ordem de cadastro, que é usada no desempate dos maiores gastos. Também atende ao requisito de usar `set` ou `list`. Não precisamos de acesso por posição; para ordenar, copiamos para um `vector`. |
| `somarGastosDoMes` | `std::map<std::string, double>` | Acumula o total por nome de categoria. O `map` cria uma entrada por categoria e permite somar com `totais[nome] += valor` sem procurar posição. |
| `maioresGastosDoMes` | `std::vector<Gasto>` temporário | O `std::sort` precisa de iteradores de acesso aleatório; `std::list` não serve. Copiamos só os gastos do mês para o `vector` e ordenamos. |
| Retorno de totais e estouradas | `std::vector<ResumoCategoria>` | É uma sequência simples de resultados para a interface percorrer e imprimir. |
| Ordenação dos maiores | `std::sort` | Usa um comparador que chama `Gasto::operator<` e, no empate, compara a `ordem` de cadastro. Assim o primeiro cadastrado aparece primeiro quando os valores são iguais. |
| Busca de categoria | `std::map::find` | A própria `Carteira` usa a busca do `map` para saber se a categoria existe. Não é necessário um `std::find` separado. |

Observação: o comparador do `std::sort` será uma função comum em
`Carteira.cpp`, sem lambda.

## 4. Arquivos do projeto

```text
include/
├── Categoria.hpp
├── Gasto.hpp
├── Carteira.hpp
└── Interface.hpp

src/
├── Categoria.cpp
├── Gasto.cpp
├── Carteira.cpp
├── Interface.cpp
└── main.cpp

exemplos/
├── basico.txt
├── erros.txt
└── maiores_gastos.txt
```

O comando do `README.md` já compila todos os `src/*.cpp`:

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
```

Não será preciso mudar o comando ao criar os arquivos acima.

## 5. Regras e mensagens que a interface vai mostrar

- Categoria repetida: `Categoria já cadastrada`.
- Categoria inexistente: `Categoria não encontrada`.
- Limite negativo: `Valor inválido`.
- Valor menor ou igual a zero: `Valor inválido`.
- Data impossível (31 em mês de 30 dias, 29/02 em ano não bissexto etc.):
  `Data inválida`.
- Mês fora de 1 a 12 ou ano menor que 1 nas consultas: `Data inválida`.
- Nenhum gasto no mês: `Nenhum gasto registrado nesse mês`.
- Nenhuma categoria estourou: `Nenhuma categoria passou do limite`.
- Letra onde era número: `Entrada inválida, tente de novo`.
- Opção de menu inexistente: `Opção inválida, tente de novo`.
- Cadastro certo: `Categoria cadastrada`, `Limite definido` ou
  `Gasto registrado`.

Formatos de saída:

- Opção 4: uma linha por categoria, na ordem alfabética do `map`, no formato
  `Mercado: 120.00 de 500.00`. Categorias sem gasto no mês aparecem com
  `0.00`.
- Opção 5: até três linhas em ordem decrescente, no formato
  `1. 120.00 - Compras da semana (22/10/2026, Mercado)`.
- Opção 6: as categorias estouradas no mesmo formato da opção 4.

## 6. Etapas de implementação

### Etapa 1 — Categoria e Gasto

Criar `Categoria.hpp/.cpp` e `Gasto.hpp/.cpp`. Criar um `src/main.cpp`
temporário que instancia uma categoria e dois gastos, imprime os getters e
testa `operator<`. Esse `main` será substituído na etapa 2.

Teste: compilar com o comando do README, rodar e conferir os dados impressos
e a comparação entre os valores.

### Etapa 2 — Carteira de categorias e menu inicial

Criar `Carteira.hpp/.cpp` com o `map` de categorias,
`cadastrarCategoria` e `definirLimite`. Criar `Interface.hpp`,
`Interface.cpp` e o `main` definitivo, com as opções 1, 2 e 0 do menu,
incluindo as mensagens de erro e a opção inválida.

Teste manual: cadastrar `Mercado`, tentar cadastrar `Mercado` de novo, definir
limite 500, tentar definir limite em categoria inexistente e limite negativo.

### Etapa 3 — Registro de gastos

Acrescentar à `Carteira` a `std::list<Gasto>`, o contador de ordem,
`registrarGasto` e a validação de data (incluindo anos bissextos). Acrescentar
a opção 3 na interface.

Teste manual: gasto válido, valor 0 e negativo, `31/04/2026`,
`29/02/2025`, `29/02/2024`, categoria inexistente e letra no lugar do valor.

### Etapa 4 — Total por categoria

Implementar `mesAnoValido`, `temGastosNoMes`, `somarGastosDoMes` e
`totaisPorCategoria`. Acrescentar a opção 4 na interface.

Teste manual: gastos em meses diferentes, categoria sem gasto no mês
mostrando `0.00`, consulta sem gastos e mês/ano inválidos.

### Etapa 5 — Maiores gastos do mês

Implementar `maioresGastosDoMes` com cópia para `std::vector`, `std::sort` e
o comparador que usa `Gasto::operator<` e a ordem de cadastro. Acrescentar a
opção 5 na interface.

Teste manual: menos de três gastos, mais de três gastos e dois gastos com o
mesmo valor (o cadastrado antes deve aparecer antes).

### Etapa 6 — Categorias acima do limite

Implementar `categoriasAcimaDoLimite` usando `Categoria::passouDoLimite`.
Acrescentar a opção 6 na interface.

Teste manual: total igual ao limite (não entra), total maior (entra), limite
0 (não entra), nenhuma categoria estourada e mês sem gastos.

### Etapa 7 — Exemplos e revisão final

Criar os arquivos de `exemplos/` e rodar todos com `./sistema < arquivo`.
Conferir:

- compilação sem avisos com `-Wall -Wextra`;
- mensagens iguais às da descrição;
- nenhuma classe do sistema usando `cin` ou `cout`;
- menu saindo com a opção 0;
- `git status` sem executáveis ou arquivos temporários.

## 7. Decisões simples que adotei (confirme se estiver certo)

- Ano válido nas consultas: qualquer inteiro maior ou igual a 1; mês de 1 a
  12.
- Nomes de categoria e descrições são lidos como linha inteira, preservando
  espaços e maiúsculas/minúsculas.
- A categoria nasce com limite 0 e o limite só muda pela opção 2.
- No registro de gasto, a ordem de validação é: valor, data e categoria.
- Na opção 6, se não houver nenhum gasto no mês, aparece
  `Nenhum gasto registrado nesse mês`; se houver gastos mas nenhum limite
  estourado, aparece `Nenhuma categoria passou do limite`.
- A opção 4 mostra todas as categorias quando existe pelo menos um gasto no
  mês, inclusive as que estão com total 0.
- A listagem das categorias segue a ordem alfabética do `map`.
