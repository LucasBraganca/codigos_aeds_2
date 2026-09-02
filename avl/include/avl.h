#ifndef AVL_H
#define AVL_H

#include <stddef.h>

typedef int chave_t;

typedef struct no_t{
    chave_t chave;
    int altura;
    struct no_t *esq;
    struct no_t *dir;
}no_t;


no_t * avl_criar(chave_t chave);

void   avl_destruir(no_t *raiz);

no_t * avl_inserir(no_t *raiz, chave_t chave);

no_t * avl_remover(no_t *raiz, chave_t chave);

no_t * avl_rotacao_esquerda(no_t *raiz);

no_t * avl_rotacao_direita(no_t *raiz);

no_t * avl_pesquisar(no_t *raiz, chave_t chave);

no_t * avl_imprime_percurso_pre_ordem(no_t *raiz);

no_t * avl_imprime_percurso_in_ordem(no_t *raiz);

no_t * avl_imprime_percurso_pos_ordem(no_t *raiz);

void avl_imprime_percurso_em_largura(no_t *raiz);

int avl_calcula_altura(no_t *raiz);

int avl_calcula_fator_de_balanceamento(no_t *raiz);


#endif
