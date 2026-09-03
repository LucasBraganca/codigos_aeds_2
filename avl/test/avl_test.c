#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "avl.h"

static int verificar_avl(no_t *raiz, int menor, int maior)
{
    if (raiz == NULL)
        return -1;

    assert(raiz->chave > menor && raiz->chave < maior);
    int he = verificar_avl(raiz->esq, menor, raiz->chave);
    int hd = verificar_avl(raiz->dir, raiz->chave, maior);
    int altura = 1 + (he > hd ? he : hd);
    int fb = he - hd;

    assert(fb >= -1 && fb <= 1);
    assert(raiz->altura == altura);
    assert(avl_calcula_altura(raiz) == altura);
    assert(avl_calcula_fator_de_balanceamento(raiz) == fb);
    return altura;
}

static no_t *inserir(const int valores[], size_t n)
{
    no_t *raiz = NULL;
    for (size_t i = 0; i < n; ++i)
        raiz = avl_inserir(raiz, valores[i]);
    return raiz;
}

static void conferir(no_t *raiz)
{
    verificar_avl(raiz, INT_MIN, INT_MAX);
}

static void conferir_tres_nos(no_t *raiz)
{
    assert(raiz->chave == 20);
    assert(raiz->esq != NULL && raiz->esq->chave == 10);
    assert(raiz->dir != NULL && raiz->dir->chave == 30);
    conferir(raiz);
}

static void test_criar_altura_e_fator(void)
{
    assert(avl_calcula_altura(NULL) == -1);
    assert(avl_calcula_fator_de_balanceamento(NULL) == 0);

    no_t *raiz = avl_criar(50);
    assert(raiz != NULL && raiz->chave == 50);
    assert(raiz->altura == 0);
    assert(raiz->esq == NULL && raiz->dir == NULL);
    conferir(raiz);
    avl_destruir(raiz);
}

static void test_inserir_pesquisar_e_duplicata(void)
{
    const int valores[] = {50, 30, 70, 20, 40, 60, 80};
    no_t *raiz = inserir(valores, 7);

    for (size_t i = 0; i < 7; ++i)
        assert(avl_pesquisar(raiz, valores[i]) != NULL);
    assert(avl_pesquisar(raiz, 100) == NULL);
    assert(avl_pesquisar(NULL, 50) == NULL);

    no_t *raiz_original = raiz;
    raiz = avl_inserir(raiz, 50);
    assert(raiz == raiz_original);
    conferir(raiz);
    avl_destruir(raiz);
}

static void test_quatro_rotacoes_de_insercao(void)
{
    const int ll[] = {30, 20, 10}, rr[] = {10, 20, 30};
    const int lr[] = {30, 10, 20}, rl[] = {10, 30, 20};
    const int *casos[] = {ll, rr, lr, rl};

    for (size_t i = 0; i < 4; ++i) {
        no_t *raiz = inserir(casos[i], 3);
        conferir_tres_nos(raiz);
        avl_destruir(raiz);
    }
}

static void test_remocoes_basicas(void)
{
    assert(avl_remover(NULL, 10) == NULL);

    const int base[] = {50, 30, 70, 20, 40, 60, 80};
    no_t *raiz = inserir(base, 7);
    raiz = avl_remover(raiz, 999);
    conferir(raiz);

    raiz = avl_remover(raiz, 20); /* folha */
    assert(avl_pesquisar(raiz, 20) == NULL);
    conferir(raiz);

    raiz = avl_remover(raiz, 30); /* agora possui somente o filho 40 */
    assert(avl_pesquisar(raiz, 30) == NULL);
    assert(avl_pesquisar(raiz, 40) != NULL);
    conferir(raiz);

    raiz = avl_remover(raiz, 70); /* dois filhos */
    assert(avl_pesquisar(raiz, 70) == NULL);
    assert(avl_pesquisar(raiz, 60) != NULL);
    assert(avl_pesquisar(raiz, 80) != NULL);
    conferir(raiz);

    chave_t antiga_raiz = raiz->chave;
    raiz = avl_remover(raiz, antiga_raiz);
    assert(avl_pesquisar(raiz, antiga_raiz) == NULL);
    conferir(raiz);
    avl_destruir(raiz);
}

static void test_quatro_rotacoes_de_remocao(void)
{
    const int ll[] = {30, 20, 40, 10, 25};
    const int lr[] = {30, 20, 40, 25};
    const int rr[] = {30, 20, 40, 35, 50};
    const int rl[] = {30, 20, 40, 35};
    const int *casos[] = {ll, lr, rr, rl};
    const size_t tamanhos[] = {5, 4, 5, 4};
    const int removidos[] = {40, 40, 20, 20};
    const int novas_raizes[] = {20, 25, 40, 35};

    for (size_t i = 0; i < 4; ++i) {
        no_t *raiz = inserir(casos[i], tamanhos[i]);
        raiz = avl_remover(raiz, removidos[i]);
        assert(raiz->chave == novas_raizes[i]);
        assert(avl_pesquisar(raiz, removidos[i]) == NULL);
        conferir(raiz);
        avl_destruir(raiz);
    }
}

int main(void)
{
    test_criar_altura_e_fator();
    test_inserir_pesquisar_e_duplicata();
    test_quatro_rotacoes_de_insercao();
    test_remocoes_basicas();
    test_quatro_rotacoes_de_remocao();
    printf("Todos os testes passaram!\n");
    return 0;
}
