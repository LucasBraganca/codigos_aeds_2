#include "blocos.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

//Essa função recebe um arquivo de saida e um vetor de blocos para realizar a intercalação desses blocos
static int intercalar_grupo(FILE *arquivo_saida, const bloco_t blocos[], int quantidade) {
  //TODO: implementar a intercalação dos blocos e salvar no arquivo.


  return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <entrada.txt> <saida.txt> <f>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *fim;
    errno = 0;
    long f = strtol(argv[3], &fim, 10);
    if (errno || fim == argv[3] || *fim != '\0' || f <= 1 || f > INT_MAX) {
        fprintf(stderr, "Erro: f deve ser um inteiro maior que 1.\n");
        return EXIT_FAILURE;
    }

    FILE *arquivo_entrada = fopen(argv[1], "r");
    if (arquivo_entrada == NULL) {
        perror("Erro ao abrir a entrada");
        return EXIT_FAILURE;
    }

    int total, tamanho;
    if (!ler_cabecalho(arquivo_entrada, &total, &tamanho) || f >= total) {
        fprintf(stderr, "Erro: cabeçalho inválido ou f >= número inicial de blocos.\n");
        fclose(arquivo_entrada);
        return EXIT_FAILURE;
    }

    bloco_t *blocos = calloc((size_t)f, sizeof(*blocos));
    if (blocos == NULL) {
        perror("Erro ao alocar o grupo");
        fclose(arquivo_entrada);
        return EXIT_FAILURE;
    }

    //Apenas para teste!
    ler_grupo(arquivo_entrada, blocos, f, tamanho);
    imprimir_blocos(blocos,f);
    liberar_blocos(blocos,f);

    /*
      TODO:  Implementar a lógica principal:
    */


    return 0;
}
