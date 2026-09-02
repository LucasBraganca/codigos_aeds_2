#include <assert.h>
#include <stdio.h>

#include "avl.h"

static void test_criar(void)
{
    no_t *raiz = avl_criar(50);

   assert(raiz != NULL);
   assert(raiz->chave == 50);
   assert(raiz->esq == NULL);
   assert(raiz->dir == NULL);

   avl_destruir(raiz);
}

static void test_inserir_esquerda(void)
{
    no_t *raiz = avl_criar(50);

    raiz = avl_inserir(raiz, 30);

    assert(raiz != NULL);
    assert(raiz->esq != NULL);

    assert(raiz->esq->chave == 30);

    avl_destruir(raiz);
}

static void test_inserir_direita(void)
{
    no_t *raiz = avl_criar(50);

    raiz = avl_inserir(raiz, 70);

    assert(raiz != NULL);
    assert(raiz->dir != NULL);

    assert(raiz->dir->chave == 70);

    avl_destruir(raiz);
}

static void test_inserir_varios(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 70);
    raiz = avl_inserir(raiz, 20);
    raiz = avl_inserir(raiz, 40);
    raiz = avl_inserir(raiz, 60);
    raiz = avl_inserir(raiz, 80);

    assert(raiz->chave == 50);

    assert(raiz->esq->chave == 30);
    assert(raiz->dir->chave == 70);

    assert(raiz->esq->esq->chave == 20);
    assert(raiz->esq->dir->chave == 40);

    assert(raiz->dir->esq->chave == 60);
    assert(raiz->dir->dir->chave == 80);

    avl_destruir(raiz);
}

static void test_pesquisar_existente(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 70);
    raiz = avl_inserir(raiz, 20);

    no_t *resultado = avl_pesquisar(raiz, 20);

    assert(resultado != NULL);
    assert(resultado->chave == 20);

    avl_destruir(raiz);
}

static void test_pesquisar_inexistente(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 70);

    no_t *resultado =
        avl_pesquisar(raiz, 100);

    assert(resultado == NULL);

    avl_destruir(raiz);
}

static void test_remover_folha(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 70);
    raiz = avl_inserir(raiz, 20);

    raiz = avl_remover(raiz, 20);

    assert(
        avl_pesquisar(raiz, 20) == NULL
    );

    assert(
        avl_pesquisar(raiz, 30) != NULL
    );

    avl_destruir(raiz);
}

static void test_remover_no_com_um_filho(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 20);

    raiz = avl_remover(raiz, 30);

    assert(
        avl_pesquisar(raiz, 30) == NULL
    );

    assert(
        avl_pesquisar(raiz, 20) != NULL
    );

    assert(raiz->esq->chave == 20);

    avl_destruir(raiz);
}

static void test_remover_no_com_dois_filhos(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 70);
    raiz = avl_inserir(raiz, 20);
    raiz = avl_inserir(raiz, 40);
    raiz = avl_inserir(raiz, 60);
    raiz = avl_inserir(raiz, 80);

    raiz = avl_remover(raiz, 70);

    assert(
        avl_pesquisar(raiz, 70) == NULL
    );

    assert(
        avl_pesquisar(raiz, 60) != NULL
    );

    assert(
        avl_pesquisar(raiz, 80) != NULL
    );

    avl_destruir(raiz);
}

static void test_remover_raiz(void)
{
    no_t *raiz = NULL;

    raiz = avl_inserir(raiz, 50);
    raiz = avl_inserir(raiz, 30);
    raiz = avl_inserir(raiz, 70);

    raiz = avl_remover(raiz, 50);

    assert(
        avl_pesquisar(raiz, 50) == NULL
    );

    assert(
        avl_pesquisar(raiz, 30) != NULL
    );

    assert(
        avl_pesquisar(raiz, 70) != NULL
    );

    avl_destruir(raiz);
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

    printf("Todos os testes passaram!\n");

    return 0;
}
