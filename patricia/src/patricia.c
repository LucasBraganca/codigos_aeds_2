#include "patricia.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

_Static_assert(D > 0, "D deve ser positivo");
_Static_assert(D <= (int)(sizeof(chave_t) * CHAR_BIT), "D nao pode exceder o numero de bits de chave_t");

struct patricia_no {
    patricia_tipo_no_t tipo;
    union {
        struct {
            int indice;
            patricia_no_t *esq;
            patricia_no_t *dir;
        } interno;
        struct {
            chave_t chave;
        } externo;
    } dados;
};

struct patricia {
    patricia_no_t *raiz;
    size_t tamanho;
};

static int chave_valida(chave_t chave)
{
    const int largura = (int)(sizeof(chave_t) * CHAR_BIT);
    if (D == largura)
        return 1;
    return chave <= (UINT_MAX >> (largura - D));
}

patricia_t *patricia_criar(void)
{
    return calloc(1, sizeof(patricia_t));
}

void patricia_destruir(patricia_t *arvore)
{
    if (arvore == NULL)
        return;
    destruir_nos(arvore->raiz);
    free(arvore);
}
ize_t patricia_tamanho(const patricia_t *arvore)
{
    return arvore == NULL ? 0 : arvore->tamanho;
}

static void imprimir_binario(chave_t chave)
{
    for (int i = 1; i <= D; ++i)
        putchar(patricia_bit(i, chave) ? '1' : '0');
}

static void imprimir_no(const patricia_no_t *no, const char *prefixo,
                        const char *ramo)
{
    printf("%s%s", prefixo, ramo);
    if (no->tipo == PATRICIA_EXTERNO) {
        imprimir_binario(no->dados.externo.chave);
        putchar('\n');
        return;
    }

    printf("[%d]\n", no->dados.interno.indice);
    char proximo[1024];
    int escritos = snprintf(proximo, sizeof proximo, "%s%s", prefixo,
                            ramo[0] == '\0' ? "" : "    ");
    if (escritos < 0 || (size_t)escritos >= sizeof proximo)
        return;
    imprimir_no(no->dados.interno.esq, proximo, "0 -> ");
    imprimir_no(no->dados.interno.dir, proximo, "1 -> ");
}

void patricia_imprimir(const patricia_t *arvore)
{
    if (arvore == NULL || arvore->raiz == NULL) {
        puts("(vazia)");
        return;
    }
    imprimir_no(arvore->raiz, "", "");
}

static int folhas_concordam_com_ramo(const patricia_no_t *no, int indice, int ramo, int profundidade)
{
    if (no == NULL || profundidade > D)
        return 0;
    if (no->tipo == PATRICIA_EXTERNO)
        return chave_valida(no->dados.externo.chave) && patricia_bit(indice, no->dados.externo.chave) == ramo;
    if (no->tipo != PATRICIA_INTERNO)
        return 0;
    return folhas_concordam_com_ramo(no->dados.interno.esq, indice, ramo, profundidade + 1) &&
           folhas_concordam_com_ramo(no->dados.interno.dir, indice, ramo, profundidade + 1);
}

static int validar_no(const patricia_no_t *no, int indice_pai, size_t *folhas, int profundidade)
{
    if (no == NULL || profundidade > D)
        return 0;
    if (no->tipo == PATRICIA_EXTERNO) {
        if (!chave_valida(no->dados.externo.chave))
            return 0;
        ++*folhas;
        return 1;
    }
    if (no->tipo != PATRICIA_INTERNO)
        return 0;

    int indice = no->dados.interno.indice;
    if (indice < 1 || indice > D || indice <= indice_pai ||
        no->dados.interno.esq == NULL || no->dados.interno.dir == NULL)
        return 0;
    if (!folhas_concordam_com_ramo(no->dados.interno.esq, indice, 0, 1) ||
        !folhas_concordam_com_ramo(no->dados.interno.dir, indice, 1, 1))
        return 0;

    return validar_no(no->dados.interno.esq, indice, folhas, profundidade + 1) &&
           validar_no(no->dados.interno.dir, indice, folhas, profundidade + 1);
}

int patricia_validar(const patricia_t *arvore)
{
    if (arvore == NULL)
        return 0;
    if (arvore->raiz == NULL)
        return arvore->tamanho == 0;
    size_t folhas = 0;
    return validar_no(arvore->raiz, 0, &folhas, 0) &&
           folhas == arvore->tamanho;
}

size_t patricia_indices_caminho(const patricia_t *arvore, chave_t chave, int *indices, size_t capacidade)
{
    if (arvore == NULL || !chave_valida(chave) ||
        (capacidade != 0 && indices == NULL))
        return 0;
    const patricia_no_t *no = arvore->raiz;
    size_t quantidade = 0;
    while (no != NULL && no->tipo == PATRICIA_INTERNO) {
        if (quantidade < capacidade)
            indices[quantidade] = no->dados.interno.indice;
        ++quantidade;
        no = patricia_bit(no->dados.interno.indice, chave)
                 ? no->dados.interno.dir : no->dados.interno.esq;
    }
    return quantidade;
}


int patricia_bit(int i, chave_t chave)
{

}

static patricia_no_t *cria_no_externo(chave_t chave)
{

}

static patricia_no_t *cria_no_interno(int indice, patricia_no_t *esq, patricia_no_t *dir)
{

}

static void destruir_nos(patricia_no_t *no)
{

}

static const patricia_no_t *encontra_folha(const patricia_no_t *no, chave_t chave)
{

}

int patricia_buscar(const patricia_t *arvore, chave_t chave)
{

}

/* Insere o novo discriminador antes do primeiro indice maior que ele.
 * Em falha de alocacao, devolve NULL sem alterar a subarvore existente. */
static patricia_no_t *insere_entre(chave_t chave, patricia_no_t *no, int indice, int *erro)
{

}

int patricia_inserir(patricia_t *arvore, chave_t chave)
{
    
}

int patricia_remover(patricia_t *arvore, chave_t chave)
{
 
}
