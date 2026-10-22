# Intercalação balanceada de blocos ordenados

Implementação em C da prática de AEDS. O programa lê grupos de até `f` blocos ordenados, intercala cada grupo e grava os resultados em disco. As passadas se repetem até produzir um único bloco com todos os valores ordenados, preservando negativos e duplicatas.

## Compilação e execução

No Linux, com GCC e Make:

```sh
make
./build/intercalacao exemplos/data.txt build/saida.txt 3
```

Ou use `make run`. Sem Make:

```sh
mkdir -p build
gcc -Wall -Wextra -Wpedantic -std=c11 -Iinclude src/main.c src/blocos.c -o build/intercalacao
./build/intercalacao exemplos/data.txt build/saida.txt 3
```

Argumentos: `<entrada.txt> <saida.txt> <f>`. A ordem deve satisfazer `1 < f < número inicial de blocos`.

## Formato dos arquivos

A entrada começa com a quantidade de blocos e a quantidade de valores por bloco. Cada linha seguinte representa um bloco já ordenado:

```text
6 4
2 8 15 21
1 7 18 30
3 5 12 27
4 6 10 20
9 11 16 25
13 14 17 19
```

Os arquivos intermediários são chamados `<saida.txt>.passada1.txt`, `<saida.txt>.passada2.txt`, etc. Não possuem cabeçalho: cada linha contém um bloco, cujo tamanho pode variar. São preservados para inspeção; o arquivo da última passada é renomeado para o caminho de saída solicitado. Arquivos anteriores com esses nomes são substituídos em uma nova execução.

O terminal informa a passada, os grupos processados, a quantidade de blocos produzidos e os arquivos gerados. Com o exemplo e `f=3`, são realizadas duas passadas: 6 blocos tornam-se 2 e depois 1.

A saída esperada, também fornecida em `exemplos/saida.txt`, é:

```text
1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 25 27 30
```

## Organização e algoritmo

- `src/main.c`: argumentos, intercalação com heap mínimo e controle das passadas.
- `src/blocos.c`: leitura e validação dos blocos iniciais e dos blocos de tamanhos variáveis, além da liberação de memória.
- `include/blocos.h`: estrutura dos blocos e interfaces.
- `exemplos/`: entrada, saída e primeira passada do exemplo.
- `docs/`: enunciado da prática.

Apenas um grupo de até `f` blocos é carregado por vez. O heap guarda o próximo valor de cada bloco ativo; a cada remoção, o valor é gravado diretamente no arquivo e o candidato daquele bloco é atualizado. Para um grupo com `N` valores, a intercalação custa `O(N log f)` e utiliza `O(f)` posições auxiliares, além da memória dos blocos carregados. Nas últimas passadas, esses blocos podem representar grande parte dos dados, conforme o modelo simplificado da prática.

São verificados argumentos, cabeçalho, quantidade de valores, ordenação interna, limites de inteiros, conteúdo extra e falhas de leitura, escrita ou alocação. As alocações são liberadas também nos caminhos de erro. `make clean` remove o executável.
