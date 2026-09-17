#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rb.h"

/* Estas verificacoes continuam ativas mesmo em builds Release (NDEBUG). */
#define CHECK(expr) do { if (!(expr)) { \
    fprintf(stderr, "%s:%d: falhou: %s\n", __FILE__, __LINE__, #expr); \
    exit(EXIT_FAILURE); \
} } while (0)

enum violacao {
    COR_INVALIDA = 1, RAIZ_VERMELHA = 2, VERMELHO_COM_VERMELHO = 4,
    ALTURA_PRETA = 8, ORDEM_BST = 16, PONTEIRO_PAI = 32
};

/* NULL representa uma folha NIL preta, com altura preta 1.
 * O limite tambem detecta ciclos antes de uma recursao ilimitada. */
static int verificar_no(const no_t *no, const no_t *pai,
                        long long min, long long max, unsigned *erros,
                        unsigned profundidade)
{
    if (no == NULL) return 1;
    if (profundidade > 256) { *erros |= PONTEIRO_PAI; return 0; }
    if (no->cor != VERMELHO && no->cor != PRETO) *erros |= COR_INVALIDA;
    if (no->pai != pai) *erros |= PONTEIRO_PAI;
    if (no->chave <= min || no->chave >= max) *erros |= ORDEM_BST;
    if (no->cor == VERMELHO &&
        ((no->esq && no->esq->cor == VERMELHO) ||
         (no->dir && no->dir->cor == VERMELHO)))
        *erros |= VERMELHO_COM_VERMELHO;
    int esq = verificar_no(no->esq, no, min, no->chave, erros, profundidade + 1);
    int dir = verificar_no(no->dir, no, no->chave, max, erros, profundidade + 1);
    if (esq != dir) *erros |= ALTURA_PRETA;
    return esq + (no->cor == PRETO);
}

static unsigned violacoes(const no_t *raiz)
{
    unsigned erros = 0;
    if (raiz && raiz->cor != PRETO) erros |= RAIZ_VERMELHA;
    verificar_no(raiz, NULL, LLONG_MIN, LLONG_MAX, &erros, 0);
    return erros;
}

static void validar(const no_t *raiz)
{
    unsigned erros = violacoes(raiz);
    if (erros) fprintf(stderr, "Propriedades violadas (mascara): %u\n", erros);
    CHECK(erros == 0);
}

static no_t *novo(int chave, cor_t cor, no_t *esq, no_t *dir)
{
    no_t *no = malloc(sizeof *no);
    CHECK(no != NULL);
    *no = (no_t){chave, cor, esq, dir, NULL};
    if (esq) esq->pai = no;
    if (dir) dir->pai = no;
    return no;
}

static void liberar(no_t *no)
{
    if (!no) return;
    liberar(no->esq);
    liberar(no->dir);
    free(no);
}

static no_t *buscar(no_t *no, int chave)
{
    while (no && no->chave != chave)
        no = chave < no->chave ? no->esq : no->dir;
    return no;
}

static size_t contar(const no_t *no)
{
    return no ? 1 + contar(no->esq) + contar(no->dir) : 0;
}

static void espelhar(no_t *no)
{
    if (!no) return;
    no->chave = 100 - no->chave;
    no_t *tmp = no->esq;
    no->esq = no->dir;
    no->dir = tmp;
    espelhar(no->esq);
    espelhar(no->dir);
}

/* Confirma que o proprio verificador detecta cada propriedade quebrada. */
static void test_propriedades(void)
{
    no_t a = {20, PRETO, NULL, NULL, NULL};
    no_t b = {10, VERMELHO, NULL, NULL, &a};
    no_t c = {5, VERMELHO, NULL, NULL, &b};
    CHECK(violacoes(NULL) == 0); /* NIL e implicitamente preto. */
    CHECK(violacoes(&a) == 0);
    a.cor = (cor_t)99;
    CHECK(violacoes(&a) & COR_INVALIDA);
    a.cor = VERMELHO;
    CHECK(violacoes(&a) == RAIZ_VERMELHA);
    a.cor = PRETO;
    a.esq = &b;
    b.esq = &c;
    CHECK(violacoes(&a) == VERMELHO_COM_VERMELHO);
    b.esq = NULL;
    b.cor = PRETO;
    CHECK(violacoes(&a) == ALTURA_PRETA);
    b.cor = VERMELHO;
    b.chave = 30;
    CHECK(violacoes(&a) == ORDEM_BST);
    b.chave = 10;
    b.pai = NULL;
    CHECK(violacoes(&a) == PONTEIRO_PAI);
    b.pai = &a;
    validar(&a);
}

static void test_criar(void)
{
    no_t *raiz = rb_criar(50);
    CHECK(raiz && raiz->chave == 50);
    CHECK(!raiz->esq && !raiz->dir && !raiz->pai);
    validar(raiz);
    rb_destruir(raiz);
    rb_destruir(NULL);
}

/* Casos classicos: raiz vermelha, tio vermelho, triangulo e linha.
 * Os casos com pai a direita sao o espelho dos casos com pai a esquerda. */
static void test_insercao(int caso, int espelho)
{
    no_t *no, *raiz;
    size_t total;
    if (caso == 0) {
        raiz = no = novo(20, VERMELHO, NULL, NULL);
        total = 1;
    } else if (caso == 1) {
        no = novo(5, VERMELHO, NULL, NULL);
        raiz = novo(20, PRETO, novo(10, VERMELHO, no, NULL),
                    novo(30, VERMELHO, NULL, NULL));
        total = 4;
    } else {
        no = novo(caso == 2 ? 15 : 5, VERMELHO, NULL, NULL);
        no_t *pai = novo(10, VERMELHO,
                        caso == 3 ? no : NULL, caso == 2 ? no : NULL);
        raiz = novo(20, PRETO, pai, NULL);
        total = 3;
    }
    if (espelho) espelhar(raiz);
    CHECK(violacoes(raiz) == (caso == 0 ? RAIZ_VERMELHA : VERMELHO_COM_VERMELHO));
    int chave = no->chave;
    raiz = rb_corrigir_insercao(raiz, no);
    CHECK(raiz != NULL);
    validar(raiz);
    CHECK(contar(raiz) == total);
    CHECK(buscar(raiz, chave) == no);
    if (caso == 1) {
        CHECK(raiz->chave == (espelho ? 80 : 20));
        CHECK(raiz->esq->cor == PRETO && raiz->dir->cor == PRETO);
    }
    if (caso >= 2) {
        CHECK(raiz->chave == (espelho ? (caso == 2 ? 85 : 90) :
                                          (caso == 2 ? 15 : 10)));
    }
    liberar(raiz);
}

/* Recolorir o tio produz outro par vermelho em um nivel superior. */
static void test_insercao_cascata(int espelho)
{
    no_t *no = novo(5, VERMELHO, NULL, NULL);
    no_t *raiz = novo(80, PRETO,
        novo(40, VERMELHO,
             novo(20, PRETO, novo(10, VERMELHO, no, NULL),
                  novo(30, VERMELHO, NULL, NULL)),
             novo(60, PRETO, NULL, NULL)),
        novo(90, PRETO, NULL, NULL));
    if (espelho) espelhar(raiz);
    CHECK(violacoes(raiz) == VERMELHO_COM_VERMELHO);
    raiz = rb_corrigir_insercao(raiz, no);
    CHECK(raiz != NULL);
    validar(raiz);
    CHECK(contar(raiz) == 8);
    const int chaves[] = {5, 10, 20, 30, 40, 60, 80, 90};
    for (size_t i = 0; i < 8; ++i)
        CHECK(buscar(raiz, espelho ? 100 - chaves[i] : chaves[i]));
    liberar(raiz);
}

static void test_insercao_pai_preto(void)
{
    no_t *no = novo(10, VERMELHO, NULL, NULL);
    no_t *raiz = novo(20, PRETO, no, NULL);
    validar(raiz);
    raiz = rb_corrigir_insercao(raiz, no);
    CHECK(raiz != NULL);
    validar(raiz);
    CHECK(contar(raiz) == 2 && buscar(raiz, 10) == no);
    liberar(raiz);
}

static void test_remocao_dois_filhos(void)
{
    /* Substituto imediato ou mais profundo, ambos os lados. A forma final
     * pode usar predecessor ou sucessor: apenas propriedades e chaves importam. */
    for (int profundo = 0; profundo < 2; ++profundo) {
        for (int espelho = 0; espelho < 2; ++espelho) {
            no_t *raiz = novo(40, PRETO,
                novo(20, PRETO, NULL, NULL),
                novo(60, PRETO,
                     profundo ? novo(50, VERMELHO, NULL, NULL) : NULL,
                     profundo ? novo(70, VERMELHO, NULL, NULL) : NULL));
            if (espelho) espelhar(raiz);
            validar(raiz);
            raiz = rb_remover(raiz, espelho ? 60 : 40);
            CHECK(raiz != NULL);
            validar(raiz);
            CHECK(contar(raiz) == (profundo ? 4u : 2u));
            CHECK(!buscar(raiz, espelho ? 60 : 40));
            const int chaves[] = {20, 60, 50, 70};
            for (int i = 0; i < (profundo ? 4 : 2); ++i)
                CHECK(buscar(raiz, espelho ? 100 - chaves[i] : chaves[i]));
            liberar(raiz);
        }
    }
}

static no_t *cenario_remocao(int caso)
{
    no_t *alvo = novo(10, PRETO, NULL, NULL);
    switch (caso) {
    case 1: /* Irmao vermelho: rotacionar e recolorir. */
        return novo(20, PRETO, alvo,
                    novo(40, VERMELHO, novo(30, PRETO, NULL, NULL),
                         novo(50, PRETO, NULL, NULL)));
    case 2: /* Irmao preto e ambos os sobrinhos pretos (NIL). */
        return novo(20, PRETO, alvo, novo(30, PRETO, NULL, NULL));
    case 3: /* Sobrinho proximo vermelho, distante preto. */
        return novo(20, PRETO, alvo,
                    novo(40, PRETO, novo(30, VERMELHO, NULL, NULL), NULL));
    case 4: /* Sobrinho distante vermelho. */
        return novo(20, PRETO, alvo,
                    novo(40, PRETO, NULL, novo(50, VERMELHO, NULL, NULL)));
    default: /* O deficit preto precisa subir mais de um nivel. */
        return novo(40, PRETO,
                    novo(20, PRETO, alvo, novo(30, PRETO, NULL, NULL)),
                    novo(60, PRETO, novo(50, PRETO, NULL, NULL),
                         novo(70, PRETO, NULL, NULL)));
    }
}

static void test_remocao(int caso, int espelho, int direto)
{
    no_t *raiz = cenario_remocao(caso);
    if (espelho) espelhar(raiz);
    validar(raiz);
    size_t total = contar(raiz);
    int chave = espelho ? 90 : 10;
    int presentes[7], quantidade = 0;
    const int candidatos[] = {10, 20, 30, 40, 50, 60, 70};
    for (size_t i = 0; i < 7; ++i) {
        int k = espelho ? 100 - candidatos[i] : candidatos[i];
        if (k != chave && buscar(raiz, k)) presentes[quantidade++] = k;
    }
    if (direto) {
        no_t *alvo = buscar(raiz, chave);
        CHECK(alvo && alvo->cor == PRETO && !alvo->esq && !alvo->dir);
        no_t *pai = alvo->pai;
        if (pai->esq == alvo) pai->esq = NULL;
        else pai->dir = NULL;
        free(alvo);
        CHECK(violacoes(raiz) == ALTURA_PRETA);
        raiz = rb_corrigir_remocao(raiz, NULL, pai);
    } else {
        raiz = rb_remover(raiz, chave);
    }
    CHECK(raiz != NULL);
    validar(raiz);
    CHECK(contar(raiz) == total - 1);
    CHECK(buscar(raiz, chave) == NULL);
    for (int i = 0; i < quantidade; ++i) CHECK(buscar(raiz, presentes[i]));
    liberar(raiz);
}

static void test_operacoes(void)
{
    const int chaves[] = {41, 38, 31, 12, 19, 8, INT_MIN, INT_MAX, 0, 50, 60};
    const size_t n = sizeof chaves / sizeof chaves[0];
    no_t *raiz = NULL;
    CHECK(rb_pesquisar(NULL, 0) == NULL);
    CHECK(rb_remover(NULL, 0) == NULL);
    for (size_t i = 0; i < n; ++i) {
        raiz = rb_inserir(raiz, chaves[i]);
        CHECK(raiz != NULL);
        validar(raiz);
        CHECK(contar(raiz) == i + 1);
        for (size_t j = 0; j <= i; ++j) {
            no_t *no = rb_pesquisar(raiz, chaves[j]);
            CHECK(no && no->chave == chaves[j]);
        }
    }
    CHECK(rb_pesquisar(raiz, 123) == NULL);
    raiz = rb_remover(raiz, 123);
    validar(raiz);
    CHECK(contar(raiz) == n);
    for (size_t i = 0; i < n; ++i) {
        raiz = rb_remover(raiz, chaves[i]);
        validar(raiz);
        CHECK(contar(raiz) == n - i - 1);
        CHECK(rb_pesquisar(raiz, chaves[i]) == NULL);
        for (size_t j = i + 1; j < n; ++j) CHECK(buscar(raiz, chaves[j]));
    }
    CHECK(raiz == NULL);
    rb_destruir(raiz);
}

static void test_remocoes_simples(void)
{
    /* Folha vermelha, no preto com filho vermelho, raiz unica. */
    for (int caso = 0; caso < 3; ++caso) {
        no_t *raiz = caso == 0 ? novo(20, PRETO, novo(10, VERMELHO, NULL, NULL), NULL) :
                      caso == 1 ? novo(20, PRETO, NULL, novo(30, VERMELHO, NULL, NULL)) :
                                  novo(20, PRETO, NULL, NULL);
        validar(raiz);
        raiz = rb_remover(raiz, caso == 0 ? 10 : 20);
        validar(raiz);
        CHECK(contar(raiz) == (caso == 2 ? 0u : 1u));
        if (caso != 2) CHECK(raiz->chave == (caso == 0 ? 20 : 30));
        liberar(raiz);
    }
}

static void test_transplantar(void)
{
    /* A operacao estrutural nao corrige cores; verifica ligacoes e pais. */
    for (int lado = 0; lado < 3; ++lado) {
        no_t *alvo = novo(20, PRETO, NULL, NULL);
        no_t *substituto = novo(25, VERMELHO, NULL, NULL);
        alvo->dir = substituto;
        substituto->pai = alvo;
        no_t *raiz = lado == 0 ? alvo :
            novo(lado == 1 ? 40 : 10, PRETO,
                 lado == 1 ? alvo : NULL, lado == 2 ? alvo : NULL);
        raiz = rb_transplantar(raiz, alvo, substituto);
        CHECK(raiz != NULL);
        if (lado == 0) {
            CHECK(raiz == substituto && raiz->pai == NULL);
        } else {
            CHECK((lado == 1 ? raiz->esq : raiz->dir) == substituto);
            CHECK(substituto->pai == raiz);
        }
        free(alvo);
        raiz = rb_transplantar(raiz, substituto, NULL);
        if (lado == 0) CHECK(raiz == NULL);
        else CHECK((lado == 1 ? raiz->esq : raiz->dir) == NULL);
        free(substituto);
        liberar(raiz);
    }
}

int main(int argc, char **argv)
{
    CHECK(argc >= 2);
    printf("Executando: %s\n", argv[1]);
    fflush(stdout);
    if (!strcmp(argv[1], "propriedades")) test_propriedades();
    else if (!strcmp(argv[1], "criar")) test_criar();
    else if (!strcmp(argv[1], "insercao_pai_preto")) test_insercao_pai_preto();
    else if (!strcmp(argv[1], "remocao_dois_filhos")) test_remocao_dois_filhos();
    else if (!strcmp(argv[1], "insercao_cascata")) {
        CHECK(argc == 3);
        test_insercao_cascata(atoi(argv[2]));
    }
    else if (!strcmp(argv[1], "operacoes")) test_operacoes();
    else if (!strcmp(argv[1], "remocoes_simples")) test_remocoes_simples();
    else if (!strcmp(argv[1], "transplantar")) test_transplantar();
    else if (!strcmp(argv[1], "insercao")) {
        CHECK(argc == 4);
        test_insercao(atoi(argv[2]), atoi(argv[3]));
    } else if (!strcmp(argv[1], "remocao")) {
        CHECK(argc == 5);
        test_remocao(atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    } else CHECK(0);
    puts("Teste passou!");
    return EXIT_SUCCESS;
}
