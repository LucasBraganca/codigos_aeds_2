#ifndef PATRICIA_H
#define PATRICIA_H

#include <stddef.h>

#ifndef D
#define D 6
#endif

typedef unsigned int chave_t;

typedef enum {
    PATRICIA_INTERNO,
    PATRICIA_EXTERNO
} patricia_tipo_no_t;

typedef struct patricia_no patricia_no_t;
typedef struct patricia patricia_t;

/* Retorna 0 ou 1. Os indices validos vao de 1 (MSB considerado) a D. */
int patricia_bit(int i, chave_t chave);

patricia_t *patricia_criar(void);
void patricia_destruir(patricia_t *arvore);

/* Retorna 1 se inseriu, 0 se era duplicada e -1 em caso de erro. */
int patricia_inserir(patricia_t *arvore, chave_t chave);
int patricia_buscar(const patricia_t *arvore, chave_t chave);

/* Extensao opcional: esta versao retorna -1 e nao modifica a arvore. */
int patricia_remover(patricia_t *arvore, chave_t chave);

void patricia_imprimir(const patricia_t *arvore);
size_t patricia_tamanho(const patricia_t *arvore);
int patricia_validar(const patricia_t *arvore);

/* Recurso de observacao para testes e depuracao, sem expor os nos.
 * Copia os indices internos do caminho da raiz ate a folha da chave.
 * Retorna o comprimento total do caminho, mesmo se capacidade for menor. */
size_t patricia_indices_caminho(const patricia_t *arvore, chave_t chave, int *indices, size_t capacidade);

#endif
