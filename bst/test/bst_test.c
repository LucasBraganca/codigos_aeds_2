#include <assert.h>
#include <stdio.h>

#include "bst.h"

static void test_criar(void)
{
    no_t *raiz = bst_criar(50);

    assert(raiz != NULL);
    assert(raiz->chave == 50);
    assert(raiz->esq == NULL);
    assert(raiz->dir == NULL);

    bst_destruir(raiz);
}

static void test_inserir_esquerda(void)
{
    no_t *raiz = bst_criar(50);

    raiz = bst_inserir(raiz, 30);

    assert(raiz != NULL);
    assert(raiz->esq != NULL);

    assert(raiz->esq->chave == 30);

    bst_destruir(raiz);
}

static void test_inserir_direita(void)
{
    no_t *raiz = bst_criar(50);

    raiz = bst_inserir(raiz, 70);

    assert(raiz != NULL);
    assert(raiz->dir != NULL);

    assert(raiz->dir->chave == 70);

    bst_destruir(raiz);
}

static void test_inserir_varios(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);
    raiz = bst_inserir(raiz, 20);
    raiz = bst_inserir(raiz, 40);
    raiz = bst_inserir(raiz, 60);
    raiz = bst_inserir(raiz, 80);

    assert(raiz->chave == 50);

    assert(raiz->esq->chave == 30);
    assert(raiz->dir->chave == 70);

    assert(raiz->esq->esq->chave == 20);
    assert(raiz->esq->dir->chave == 40);

    assert(raiz->dir->esq->chave == 60);
    assert(raiz->dir->dir->chave == 80);

    bst_destruir(raiz);
}

static void test_pesquisar_existente(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);
    raiz = bst_inserir(raiz, 20);

    no_t *resultado = bst_pesquisar(raiz, 20);

    assert(resultado != NULL);
    assert(resultado->chave == 20);

    bst_destruir(raiz);
}

static void test_pesquisar_inexistente(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);

    no_t *resultado =
    bst_pesquisar(raiz, 100);

    assert(resultado == NULL);

    bst_destruir(raiz);
}

static void test_remover_folha(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);
    raiz = bst_inserir(raiz, 20);

    raiz = bst_remover(raiz, 20);

    assert(
        bst_pesquisar(raiz, 20) == NULL
    );

    assert(
        bst_pesquisar(raiz, 30) != NULL
    );

    bst_destruir(raiz);
}

static void test_remover_no_com_um_filho(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 20);

    raiz = bst_remover(raiz, 30);

    assert(
        bst_pesquisar(raiz, 30) == NULL
    );

    assert(
        bst_pesquisar(raiz, 20) != NULL
    );

    assert(raiz->esq->chave == 20);

    bst_destruir(raiz);
}

static void test_remover_no_com_dois_filhos(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);
    raiz = bst_inserir(raiz, 20);
    raiz = bst_inserir(raiz, 40);
    raiz = bst_inserir(raiz, 60);
    raiz = bst_inserir(raiz, 80);

    raiz = bst_remover(raiz, 70);

    assert(
        bst_pesquisar(raiz, 70) == NULL
    );

    assert(
        bst_pesquisar(raiz, 60) != NULL
    );

    assert(
        bst_pesquisar(raiz, 80) != NULL
    );

    bst_destruir(raiz);
}

static void test_remover_raiz(void)
{
    no_t *raiz = NULL;

    raiz = bst_inserir(raiz, 50);
    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);

    raiz = bst_remover(raiz, 50);

    assert(
        bst_pesquisar(raiz, 50) == NULL
    );

    assert(
        bst_pesquisar(raiz, 30) != NULL
    );

    assert(
        bst_pesquisar(raiz, 70) != NULL
    );

    bst_destruir(raiz);
}

static void test_calcular_altura(void)
{
    no_t *raiz = NULL;

    assert(bst_calcula_altura(raiz) == -1);

    raiz = bst_inserir(raiz, 50);
    assert(bst_calcula_altura(raiz) == 0);

    raiz = bst_inserir(raiz, 30);
    raiz = bst_inserir(raiz, 70);
    raiz = bst_inserir(raiz, 20);
    assert(bst_calcula_altura(raiz) == 2);

    bst_destruir(raiz);
}

static void test_operacoes_em_arvore_vazia(void)
{
    assert(bst_pesquisar(NULL, 10) == NULL);
    assert(bst_remover(NULL, 10) == NULL);
    assert(bst_imprime_percurso_pre_ordem(NULL) == NULL);
    assert(bst_imprime_percurso_in_ordem(NULL) == NULL);
    assert(bst_imprime_percurso_pos_ordem(NULL) == NULL);
    bst_imprime_percurso_em_largura(NULL);
    bst_destruir(NULL);
}

static void test_ignorar_chave_duplicada(void)
{
    no_t *raiz = bst_criar(50);
    no_t *raiz_original = raiz;

    raiz = bst_inserir(raiz, 50);

    assert(raiz == raiz_original);
    assert(raiz->esq == NULL && raiz->dir == NULL);
    bst_destruir(raiz);
}

static void test_remover_unico_no(void)
{
    no_t *raiz = bst_criar(50);
    raiz = bst_remover(raiz, 50);
    assert(raiz == NULL);
}

int main(void)
{
    test_criar();

    test_inserir_esquerda();
    test_inserir_direita();
    test_inserir_varios();

    test_pesquisar_existente();
    test_pesquisar_inexistente();

    test_remover_folha();
    test_remover_no_com_um_filho();
    test_remover_no_com_dois_filhos();
    test_remover_raiz();

    test_calcular_altura();
    test_operacoes_em_arvore_vazia();
    test_ignorar_chave_duplicada();
    test_remover_unico_no();

    printf("Todos os testes passaram!\n");

    return 0;
}
