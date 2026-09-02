# Projeto TAD Árvore Bínaria de Pesquisa AVL

Estrutura:

```text
avl/
├── CMakeLists.txt
├── README.md
├── include/
│   └── avl.h
├── src/
│   ├── avl.c
│   └── main.c
└── test/
    └── avl_test.c
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
