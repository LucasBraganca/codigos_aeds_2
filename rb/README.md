# Projeto TAD Árvore Bínaria de Pesquisa Vermelho-Preto

Estrutura:

```text
rb/
├── CMakeLists.txt
├── README.md
├── include/
│   └── rb.h
├── src/
│   ├── rb.c
│   └── main.c
└── test/
    └── rb_test.c
```

## Compilação

```bash
cmake -S . -B build
cmake --build build
```

## Executar o programa

```bash
./build/programa
```

## Executar os testes

```bash
ctest --test-dir build --output-on-failure
```

## Cobertura dos testes Red-Black

`NULL` representa uma folha NIL preta. O verificador confere cores validas,
raiz preta, ausencia de pai/filho vermelhos, altura preta igual em todos os
caminhos, ordem BST e ponteiros `pai`. Um teste independente cria violacoes
intencionais para verificar cada uma dessas verificacoes.

Os casos de insercao incluem raiz vermelha, pai preto, tio vermelho,
triangulo, linha e recoloracao em cascata, com casos espelhados.
Os casos de remocao incluem irmao vermelho, irmao preto com sobrinhos
pretos, sobrinho proximo vermelho, sobrinho distante vermelho e propagacao
ate niveis superiores. Cada caso testa tanto `rb_remover` quanto
`rb_corrigir_remocao` diretamente, nos dois lados da arvore.

Tambem ha testes para folha vermelha, no preto com filho vermelho,
raiz unica, no com dois filhos, transplante (raiz e ambos os lados, com
substituto ou NULL), pesquisa, chaves extremas e sequencias de operacoes.
Os testes nao exigem uma forma unica para toda arvore balanceada.

Cada caso esta registrado separadamente no CTest. Para executar apenas
as verificacoes do proprio validador:

```bash
ctest --test-dir build -R '^RB_propriedades$' --output-on-failure
```

Os corpos dos metodos em `src/rb.c` ainda estao vazios. A suite compila,
mas os testes de operacoes devem falhar ate a implementacao do TAD.
