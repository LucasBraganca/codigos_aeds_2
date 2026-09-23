#include "patricia.h"

#include <assert.h>
#include <stdio.h>

#define B 18U /* 010010 */
#define C 19U /* 010011 */
#define H 24U /* 011000 */
#define J 33U /* 100001 */
#define Q 40U /* 101000 */
#define K 34U /* 100010 */
#define W 54U /* 110110 */

static void inserir_e_validar(patricia_t *arvore, chave_t chave)
{
    assert(patricia_inserir(arvore, chave) == 1);
    assert(patricia_validar(arvore));
}

static void test_bits(void)
{
    const int esperado[] = {0, 1, 0, 0, 1, 0};
    for (int i = 1; i <= D; ++i)
        assert(patricia_bit(i, B) == esperado[i - 1]);
    assert(patricia_bit(0, B) == 0);
    assert(patricia_bit(D + 1, B) == 0);
}

static void test_arvore_vazia(void)
{
    patricia_t *arvore = patricia_criar();
    assert(arvore != NULL);
    assert(patricia_tamanho(arvore) == 0);
    assert(!patricia_buscar(arvore, B));
    assert(patricia_validar(arvore));
    patricia_destruir(arvore);
}

static void test_primeira_insercao(void)
{
    patricia_t *arvore = patricia_criar();
    inserir_e_validar(arvore, B);
    assert(patricia_tamanho(arvore) == 1);
    assert(patricia_buscar(arvore, B));
    assert(patricia_indices_caminho(arvore, B, NULL, 0) == 0);
    patricia_destruir(arvore);
}

static void test_chave_duplicada(void)
{
    patricia_t *arvore = patricia_criar();
    inserir_e_validar(arvore, B);
    assert(patricia_inserir(arvore, B) == 0);
    assert(patricia_tamanho(arvore) == 1);
    assert(patricia_validar(arvore));
    patricia_destruir(arvore);
}

static void test_duas_chaves(void)
{
    patricia_t *arvore = patricia_criar();
    inserir_e_validar(arvore, B);
    inserir_e_validar(arvore, C);
    int caminho_b[1], caminho_c[1];
    assert(patricia_indices_caminho(arvore, B, caminho_b, 1) == 1);
    assert(patricia_indices_caminho(arvore, C, caminho_c, 1) == 1);
    assert(caminho_b[0] == 6 && caminho_c[0] == 6);
    assert(patricia_buscar(arvore, B));
    assert(patricia_buscar(arvore, C));
    patricia_destruir(arvore);
}

static patricia_t *criar_exemplo_sem_k(void)
{
    patricia_t *arvore = patricia_criar();
    assert(arvore != NULL);
    const chave_t chaves[] = {B, C, H, J, Q};
    for (size_t i = 0; i < sizeof chaves / sizeof chaves[0]; ++i)
        inserir_e_validar(arvore, chaves[i]);
    return arvore;
}

static void test_exemplo_ziviani(void)
{
    patricia_t *arvore = criar_exemplo_sem_k();
    const chave_t chaves[] = {B, C, H, J, Q};
    for (size_t i = 0; i < sizeof chaves / sizeof chaves[0]; ++i)
        assert(patricia_buscar(arvore, chaves[i]));
    assert(!patricia_buscar(arvore, 0U));
    assert(!patricia_buscar(arvore, 1U));
    assert(!patricia_buscar(arvore, 63U));
    assert(patricia_tamanho(arvore) == 5);
    assert(patricia_validar(arvore));
    patricia_destruir(arvore);
}

static void test_insercao_k(void)
{
    patricia_t *arvore = criar_exemplo_sem_k();
    inserir_e_validar(arvore, K);
    int caminho_j[3], caminho_k[3];
    assert(patricia_indices_caminho(arvore, J, caminho_j, 3) == 3);
    assert(patricia_indices_caminho(arvore, K, caminho_k, 3) == 3);
    assert(caminho_j[0] == 1 && caminho_j[1] == 3 && caminho_j[2] == 5);
    assert(caminho_k[0] == 1 && caminho_k[1] == 3 && caminho_k[2] == 5);
    assert(patricia_tamanho(arvore) == 6);
    patricia_destruir(arvore);
}

static void test_insercao_w(void)
{
    patricia_t *arvore = criar_exemplo_sem_k();
    inserir_e_validar(arvore, K);
    inserir_e_validar(arvore, W);

    int caminho_w[2], caminho_j[4], caminho_q[3];
    assert(patricia_indices_caminho(arvore, W, caminho_w, 2) == 2);
    assert(caminho_w[0] == 1 && caminho_w[1] == 2);

    assert(patricia_indices_caminho(arvore, J, caminho_j, 4) == 4);
    assert(caminho_j[0] == 1 && caminho_j[1] == 2);
    assert(caminho_j[2] == 3 && caminho_j[3] == 5);

    assert(patricia_indices_caminho(arvore, Q, caminho_q, 3) == 3);
    assert(caminho_q[0] == 1 && caminho_q[1] == 2 && caminho_q[2] == 3);
    assert(patricia_tamanho(arvore) == 7);
    assert(patricia_validar(arvore));
    patricia_destruir(arvore);
}

static void test_varias_buscas_inexistentes(void)
{
    patricia_t *arvore = criar_exemplo_sem_k();
    assert(!patricia_buscar(arvore, 0U));
    assert(!patricia_buscar(arvore, 1U));
    assert(!patricia_buscar(arvore, 63U));
    assert(!patricia_buscar(arvore, K));
    patricia_destruir(arvore);
}

static void test_tamanho_e_ordens_diferentes(void)
{
    const chave_t chaves[] = {W, K, Q, J, H, C, B};
    patricia_t *arvore = patricia_criar();
    for (size_t i = 0; i < sizeof chaves / sizeof chaves[0]; ++i) {
        inserir_e_validar(arvore, chaves[i]);
        assert(patricia_tamanho(arvore) == i + 1);
    }
    for (size_t i = 0; i < sizeof chaves / sizeof chaves[0]; ++i)
        assert(patricia_buscar(arvore, chaves[i]));
    patricia_destruir(arvore);
}

static void test_entradas_invalidas_e_destruicao(void)
{
    assert(!patricia_buscar(NULL, B));
    assert(patricia_tamanho(NULL) == 0);
    assert(!patricia_validar(NULL));
    assert(patricia_inserir(NULL, B) == -1);
    patricia_destruir(NULL);

    patricia_t *arvore = patricia_criar();
    assert(arvore != NULL);
#if D < 32
    assert(patricia_inserir(arvore, (chave_t)1U << D) == -1);
    assert(!patricia_buscar(arvore, (chave_t)1U << D));
#endif
    assert(patricia_remover(arvore, B) == -1);
    assert(patricia_tamanho(arvore) == 0);
    patricia_destruir(arvore);
}

int main(void)
{
    test_bits();
    test_arvore_vazia();
    test_primeira_insercao();
    test_chave_duplicada();
    test_duas_chaves();
    test_exemplo_ziviani();
    test_insercao_k();
    test_insercao_w();
    test_varias_buscas_inexistentes();
    test_tamanho_e_ordens_diferentes();
    test_entradas_invalidas_e_destruicao();
    puts("Todos os testes da Arvore Patricia passaram!");
    return 0;
}
