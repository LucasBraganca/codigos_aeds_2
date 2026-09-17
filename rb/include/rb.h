#ifndef RB_H
#define RB_H

#include <stddef.h>

typedef int chave_t;
typedef enum {VERMELHO,PRETO} cor_t;

typedef struct no_t{
    chave_t chave;
    cor_t cor;
    struct no_t *esq;
    struct no_t *dir;
    struct no_t *pai;
}no_t;


no_t * rb_criar(chave_t chave);

void   rb_destruir(no_t *raiz);

no_t * rb_inserir(no_t *raiz, chave_t chave);

no_t * rb_remover(no_t *raiz, chave_t chave);

no_t * rb_corrigir_insercao(no_t *raiz, no_t *no);

/* pai permite corrigir a remocao quando no == NULL. */
no_t * rb_corrigir_remocao(no_t *raiz, no_t *no, no_t *pai);

/* Substitui o no atual pelo substituto e retorna a raiz atualizada. */
no_t * rb_transplantar(no_t *raiz, no_t *atual, no_t *substituto);

no_t * rb_rotacao_esquerda(no_t *raiz);

no_t * rb_rotacao_direita(no_t *raiz);

no_t * rb_pesquisar(no_t *raiz, chave_t chave);

no_t * rb_imprime_percurso_pre_ordem(no_t *raiz);

no_t * rb_imprime_percurso_in_ordem(no_t *raiz);

no_t * rb_imprime_percurso_pos_ordem(no_t *raiz);

void rb_imprime_percurso_em_largura(no_t *raiz);

int rb_calcula_altura(no_t *raiz);



#endif
