#ifndef BLOCOS_H
#define BLOCOS_H

#include <stdio.h>

typedef struct {
    int *dados;
    int tamanho;
} bloco_t;

/* Formato inicial: cabeçalho e blocos de tamanho uniforme. Retorna 1 no sucesso. */
int ler_cabecalho(FILE *arquivo, int *quantidade, int *tamanho);
/* O vetor deve estar zerado. Em caso de erro, liberar também o grupo parcial. */
int ler_grupo(FILE *arquivo, bloco_t blocos[], int quantidade, int tamanho);
/* Lê blocos de tamanhos variáveis, sem cabeçalho. */
int ler_grupo_variavel(FILE *arquivo, bloco_t blocos[], int quantidade);
void imprimir_blocos(const bloco_t blocos[], int quantidade);
/* Libera os dados e zera os blocos, mantendo o vetor disponível para reúso. */
void liberar_blocos(bloco_t blocos[], int quantidade);

#endif
