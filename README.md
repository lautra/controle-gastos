# Controle de Gastos

Atividade da Unidade 2 de Linguagem de Programação I. Leia `ENUNCIADO.md`
antes de começar.

Programa de terminal que controla gastos por categoria, mostra os maiores gastos do mês e as categorias que passaram do limite. A descrição completa está em
`DESCRICAO.md`.

## Arquivos

```text
.
├── AGENTS.md       regras que a IA lê antes de cada pedido
├── DESCRICAO.md    a descrição do sistema, escrita por você
├── ENUNCIADO.md    a atividade
├── PLANO.md        o plano feito com a IA (criado na etapa 3)
├── README.md       este arquivo
├── exemplos/       arquivos de entrada para testar
├── include/        um .hpp por classe
└── src/            um .cpp por classe, mais a interface
```

## Compilar e executar

```bash
g++ -std=c++11 -Wall -Wextra src/*.cpp -Iinclude -o sistema
./sistema
```

Se o comando mudar, atualize esta seção. Na correção eu vou usar o que
estiver escrito aqui.

## Onde uso a STL

| Onde | O quê | Por quê |
| --- | --- | --- |
| `Carteira::categorias` | `map<string, Categoria>` | Uso map porque busco a categoria pelo nome e o nome não pode se repetir. Não usei o vector porque eu teria de percorrer tudo para achar a categoria e de novo para conferir se o nome já existe. |
| `Carteira::gastos` | `list<Gasto>` | Guardo os gastos na ordem de registro; só acrescento no fim e percorro. |
| `somarGastosDoMes` | `map<string, double>` | Associo cada categoria ao total do mês; `totais[nome] += valor` já cria a entrada. |
| `maioresGastosDoMes` | `vector<Gasto>` + `sort` | O `sort` precisa de acesso por posição, que a `list` não tem; ordeno pelo `operator<` do `Gasto`. |

## Como usei a IA

- Modelo usado: Big Pickle, no OpenCode.
- Um pedido que deu certo, e por que você acha que deu certo: “Implemente somente a etapa N do @PLANO.md. Antes de editar, me diga quais arquivos vai criar ou alterar.” Ele listava os arquivos antes, e eu conferia se estava dentro do plano.
- Um pedido que deu errado, o que veio errado e como você mudou o pedido: Na etapa 7, o erros.txt veio sem os casos de letra no lugar de número e de opção 9. Pedi para acrescentar só esses dois, e passou a mostrar Opção inválida e Entrada inválida.
- Uma coisa que a IA sugeriu e você recusou, e por quê: O plano do OpenCode usou list para os gastos. Notei que vector também serviria, já que só acrescento no fim e percorro. Mantive o list porque cumpre o requisito ‘set ou list’ e porque guarda a ordem de registro, usada no desempate dos maiores gastos. Para ordenar, copio para um vector, já que o sort não funciona na list.

