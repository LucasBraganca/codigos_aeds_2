# Projeto TAD Árvore Bínaria de Pesquisa

Estrutura:

```text
bst/
├── CMakeLists.txt
├── README.md
├── include/
│   └── bst.h
├── src/
│   ├── bst.c
│   └── main.c
└── test/
    └── bst_test.c
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
