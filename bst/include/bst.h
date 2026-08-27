#ifndef BST_H
#define BST_H

#include <stddef.h>

typedef int chave_t;

typedef struct no_t{
    chave_t chave;
    struct no_t *esq;
    struct no_t *dir;
}no_t;


no_t * bst_criar(chave_t chave);

void bst_destruir(no_t *raiz);

no_t * bst_inserir(no_t *raiz, chave_t chave);

no_t * bst_remover(no_t *raiz, chave_t chave);

no_t * bst_pesquisar(no_t *raiz, chave_t chave);

no_t * bst_imprime_percurso_pre_ordem(no_t *raiz);

no_t * bst_imprime_percurso_in_ordem(no_t *raiz);

no_t * bst_imprime_percurso_pos_ordem(no_t *raiz);

void bst_imprime_percurso_em_largura(no_t *raiz);

int bst_calcula_altura(no_t *raiz);

#endif
