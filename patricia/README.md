# Árvore Patricia com chaves binárias

Este projeto implementa, em C, o TAD de Árvore Patricia apresentado em sala. A árvore guarda
chaves binárias de tamanho fixo sem usar strings e separa explicitamente nós
internos de nós externos.

## Organização

```text
patricia/
├── include/
│   └── patricia.h
├── src/
│   ├── main.c
│   └── patricia.c
├── tests/
│   └── test_patricia.c
├── Makefile
└── README.md
```

A estrutura dos nós fica privada em `patricia.c`. Assim, quem usa o TAD opera
por meio de `patricia_t` e das funções declaradas em `patricia.h`, sem alterar
ponteiros ou campos internos diretamente.

## Ideia da Patricia

Uma Patricia evita os níveis desnecessários de uma trie binária comum. Ela tem
dois tipos de nó:

- **interno:** contém apenas o índice do bit que decide o próximo ramo e dois
  filhos;
- **externo:** representa uma folha e contém a chave completa.

O ramo esquerdo representa bit `0`; o direito representa bit `1`. Como uma
chave fica somente em uma folha, chegar a uma folha não prova que a busca teve
sucesso: ainda é obrigatório comparar a chave completa.

## Chaves e numeração dos bits

O tipo da chave é:

```c
typedef unsigned int chave_t;
```

`D`, definido por padrão como `6` em `patricia.h`, informa quantos bits são
considerados. Pode-se compilar com outro valor compatível com a largura de
`chave_t`, por exemplo acrescentando `-DD=8` às opções do compilador.
Somente valores entre `0` e `2^D - 1` são aceitos quando `D` é menor que a
largura do tipo.

Os índices começam em 1 e são contados da esquerda para a direita:

```text
posição:  1 2 3 4 5 6
010010:   0 1 0 0 1 0
```

A função `patricia_bit(i, chave)` desloca a chave em `D - i` posições e aplica
uma máscara de um bit. Portanto, `patricia_bit(1, chave)` lê o bit mais
significativo considerado, e `patricia_bit(D, chave)` lê o menos significativo.
Para um índice fora de `1..D`, a função retorna zero.

## Busca

A busca começa na raiz. Em cada nó interno de índice `i`, chama
`patricia_bit(i, chave)`: zero segue para a esquerda e um segue para a direita.
Ao alcançar um nó externo, compara a chave procurada com a chave completa da
folha. Por exemplo, chegar à folha `100010` ao procurar `100011` significa que
a chave não existe.

## Inserção e `insere_entre`

A primeira chave vira diretamente um nó externo. Nas demais inserções, o
algoritmo executa quatro etapas:

1. percorre os discriminadores até encontrar uma folha;
2. compara a chave inteira para rejeitar duplicatas;
3. procura, de 1 até `D`, o primeiro bit diferente entre a chave nova e a
   chave da folha;
4. chama `insere_entre` com esse índice.

`insere_entre` percorre novamente a árvore. Se encontrar uma folha, ou um nó
interno cujo índice seja maior que o novo índice, cria naquele ponto um nó
interno e uma nova folha. Caso contrário, continua pelo ramo indicado pela
chave. Essa etapa é indispensável: colocar o discriminador sempre acima da
folha encontrada pode deixar os índices fora de ordem.

Em qualquer caminho da raiz até uma folha vale o invariante:

```text
indice do pai < indice do filho interno
```

A função pública retorna `1` quando insere, `0` quando a chave já existe e
`-1` para entrada inválida ou falha de memória.

## Exemplo de Ziviani

Usamos estas chaves de seis bits:

```text
B = 010010 = 18    C = 010011 = 19
H = 011000 = 24    J = 100001 = 33
Q = 101000 = 40    K = 100010 = 34
W = 110110 = 54
```

Após inserir `B, C, H, J, Q, K`, a estrutura é:

```text
                         [1]
                       /     \
                    [3]       [3]
                   /   \     /   \
                 [6]    H  [5]    Q
                /  \       / \
               B    C     J   K
```

Ao buscar a posição de `W`, pode-se chegar a `K`. O primeiro bit diferente
entre elas é 2:

```text
posição: 1 2 3 4 5 6
K:       1 0 0 0 1 0
W:       1 1 0 1 1 0
           ^
```

O índice 2 precisa entrar antes do índice 3 que já existe no ramo direito:

```text
                              [1]
                            /     \
                          [3]      [2]
                         /   \     /  \
                       [6]    H  [3]   W
                      /  \       / \
                     B    C    [5]  Q
                              /   \
                             J     K
```

O teste `test_insercao_w` verifica explicitamente os caminhos `1,2` para `W`,
`1,2,3` para `Q` e `1,2,3,5` para `J`. Assim ele detecta uma versão incorreta
que coloque o novo nó apenas acima da folha.

## Impressão e validação

`patricia_imprimir` mostra discriminadores entre colchetes, ramos `0` e `1`, e
folhas em binário com exatamente `D` posições. Exemplo simplificado:

```text
[6]
0 -> 010010
1 -> 010011
```

`patricia_validar` verifica:

- tipos válidos e número de folhas igual ao tamanho registrado;
- dois filhos não nulos em cada nó interno;
- índices dentro de `1..D` e estritamente crescentes;
- chaves dentro da faixa configurada;
- todas as folhas de um ramo coerentes com o bit do discriminador.

`patricia_indices_caminho` é um recurso somente de observação, útil para testes
e depuração sem expor a representação privada dos nós.

## Compilação e testes

```bash
make
make test
```

O alvo padrão também gera `programa`, que oferece um menu interativo:

```bash
./programa
```

As chaves são informadas em decimal. A impressão exibe cada chave em binário
com exatamente `D` bits. A opção de remoção informa que ela permanece como
extensão opcional.

Saída esperada dos testes:

```text
./test_patricia
Todos os testes da Arvore Patricia passaram!
```

Para apagar objetos e executável:

```bash
make clean
```

Para procurar vazamentos e acessos inválidos, se Valgrind estiver instalado:

```bash
valgrind --leak-check=full ./test_patricia
# ou
make valgrind
```

A destruição percorre os dois filhos dos nós internos, libera todas as folhas e
por fim libera a estrutura principal. `patricia_destruir(NULL)` também é
permitido.

## Remoção

A remoção foi mantida como extensão opcional para preservar o foco da aula em
busca, inserção e `InsereEntre`. `patricia_remover` está declarada para manter a
interface proposta, retorna `-1` e não modifica a árvore. Os testes obrigatórios
não dependem dela.

## Erros comuns

- **Confundir índice e valor do bit:** `indice = 3` indica qual posição ler; o
  resultado de `patricia_bit(3, chave)` é que vale zero ou um.
- **Começar os índices em zero:** este algoritmo usa `1..D`.
- **Inverter os ramos:** zero sempre vai à esquerda e um à direita.
- **Aceitar qualquer folha encontrada:** a chave completa da folha precisa ser
  comparada com a chave procurada.
- **Inserir sempre acima da folha:** `insere_entre` deve voltar desde a raiz e
  localizar a posição ordenada do novo índice.
- **Quebrar a ordem dos índices:** os índices internos devem crescer
  estritamente em todo caminho.
- **Ignorar duplicatas:** uma duplicata não cria folha nem altera o tamanho.
- **Calcular o bit mais significativo incorretamente:** para índices em `1..D`,
  o deslocamento correto é `D - i`.
- **Usar strings para os bits:** as chaves são inteiros; strings aparecem apenas
  conceitualmente na documentação e na saída.
- **Esquecer memória alocada:** todo nó e a estrutura `patricia_t` precisam ser
  liberados por `patricia_destruir`.
