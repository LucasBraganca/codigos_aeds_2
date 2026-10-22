#include "blocos.h"

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

/* Lê exatamente uma linha, respeitando a fronteira de cada bloco.
 * Aceita CRLF e a última linha sem quebra de linha. */
static int ler_linha(FILE *arquivo, int valores[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        int c;
        do {
            c = fgetc(arquivo);
        } while (c == ' ' || c == '\t' || c == '\r');
        if (c == '\n' || c == EOF) {
            return 0;
        }
        char token[64];
        size_t n = 0;
        while (c != EOF && c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            if (n == sizeof(token) - 1) {
                return 0;
            }
            token[n++] = (char)c;
            c = fgetc(arquivo);
        }
        if (c != EOF) {
            ungetc(c, arquivo);
        }
        token[n] = '\0';
        char *fim;
        errno = 0;
        long valor = strtol(token, &fim, 10);
        if (errno || fim == token || fim != token + n || valor < INT_MIN || valor > INT_MAX) {
            return 0;
        }
        valores[i] = (int)valor;
    }
    int c;
    do {
        c = fgetc(arquivo);
    } while (c == ' ' || c == '\t' || c == '\r');
    return !ferror(arquivo) && (c == '\n' || c == EOF);
}

int ler_cabecalho(FILE *arquivo, int *quantidade, int *tamanho) {
    int cabecalho[2];
    if (!ler_linha(arquivo, cabecalho, 2) || cabecalho[0] <= 0 || cabecalho[1] <= 0) {
        return 0;
    }
    *quantidade = cabecalho[0];
    *tamanho = cabecalho[1];
    return 1;
}

int ler_grupo(FILE *arquivo, bloco_t blocos[], int quantidade, int tamanho) {
    if (tamanho <= 0 || (size_t)tamanho > SIZE_MAX / sizeof(int)) {
        return 0;
    }
    for (int i = 0; i < quantidade; i++) {
        blocos[i].dados = malloc((size_t)tamanho * sizeof(*blocos[i].dados));
        if (blocos[i].dados == NULL) {
            return 0;
        }
        blocos[i].tamanho = tamanho;
        if (!ler_linha(arquivo, blocos[i].dados, tamanho)) {
            return 0;
        }
        for (int j = 1; j < tamanho; j++) {
            if (blocos[i].dados[j] < blocos[i].dados[j - 1]) {
                return 0;
            }
        }
    }
    return 1;
}

void imprimir_blocos(const bloco_t blocos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        for (int j = 0; j < blocos[i].tamanho; j++) {
            printf("%s%d", j == 0 ? "" : " ", blocos[i].dados[j]);
        }
        putchar('\n');
    }
}

int ler_grupo_variavel(FILE *arquivo, bloco_t blocos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        size_t capacidade = 0;
        bloco_t *bloco = &blocos[i];
        int c;
        for (;;) {
            do {
                c = fgetc(arquivo);
            } while (c == ' ' || c == '\t' || c == '\r');
            if (c == '\n' || c == EOF) {
                if (bloco->tamanho == 0 || ferror(arquivo)) {
                    return 0;
                }
                break;
            }
            char token[64];
            size_t n = 0;
            do {
                if (n == sizeof(token) - 1) {
                    return 0;
                }
                token[n++] = (char)c;
                c = fgetc(arquivo);
            } while (c != EOF && c != ' ' && c != '\t' && c != '\r' && c != '\n');
            if (c != EOF) {
                ungetc(c, arquivo);
            }
            token[n] = '\0';
            char *fim;
            errno = 0;
            long valor = strtol(token, &fim, 10);
            if (errno || fim != token + n || valor < INT_MIN || valor > INT_MAX ||
                bloco->tamanho == INT_MAX ||
                (bloco->tamanho > 0 && valor < bloco->dados[bloco->tamanho - 1])) {
                return 0;
            }
            if ((size_t)bloco->tamanho == capacidade) {
                size_t limite = SIZE_MAX / sizeof(*bloco->dados);
                if (limite > INT_MAX) {
                    limite = INT_MAX;
                }
                size_t nova = capacidade == 0 ? 64 : capacidade * 2;
                if (nova > limite) {
                    nova = limite;
                }
                if (nova <= capacidade) {
                    return 0;
                }
                int *dados = realloc(bloco->dados, nova * sizeof(*dados));
                if (dados == NULL) {
                    return 0;
                }
                bloco->dados = dados;
                capacidade = nova;
            }
            bloco->dados[bloco->tamanho++] = (int)valor;
        }
    }
    return 1;
}

void liberar_blocos(bloco_t blocos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        free(blocos[i].dados);
        blocos[i].dados = NULL;
        blocos[i].tamanho = 0;
    }
}
