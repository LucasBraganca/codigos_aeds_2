#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

static void imprimir_menu(void)
{
    printf("\n");
    printf("============================\n");
    printf("      ÁRVORE BST\n");
    printf("============================\n");
    printf("1 - Criar arvore\n");
    printf("2 - Inserir chave\n");
    printf("3 - Remover chave\n");
    printf("4 - Pesquisar chave\n");
    printf("5 - Imprimir bst\n");
    printf("6 - Calcular altura\n");
    printf("7 - Destruir arvore\n");
    printf("0 - Sair\n");
    printf("============================\n");
    printf("Opcao: ");
}

int main(void)
{
    no_t *raiz = NULL;

    int opcao;
    chave_t chave;

    do {

        imprimir_menu();

        scanf("%d", &opcao);

        switch (opcao) {

        case 1:

            if (raiz != NULL) {
                printf("A arvore ja foi criada.\n");
                break;
            }

            printf("Digite a chave da raiz: ");
            scanf("%d", &chave);

            raiz = bst_criar(chave);

            if (raiz == NULL) {
                printf("Erro ao criar a arvore.\n");
            }
            else {
                printf("Arvore criada com sucesso.\n");
            }

            break;

        case 2:

            printf("Digite a chave para inserir: ");
            scanf("%d", &chave);

            raiz = bst_inserir(raiz, chave);

            printf("Operacao de insercao realizada.\n");

            break;

        case 3:

            printf("Digite a chave para remover: ");
            scanf("%d", &chave);

            raiz = bst_remover(raiz, chave);

            printf("Operacao de remocao realizada.\n");

            break;

        case 4: {

            printf("Digite a chave para pesquisar: ");
            scanf("%d", &chave);

            no_t *resultado =
                bst_pesquisar(raiz, chave);

            if (resultado != NULL) {
                printf(
                    "Chave %d encontrada.\n",
                    resultado->chave
                );
            }
            else {
                printf(
                    "Chave %d nao encontrada.\n",
                    chave
                );
            }

            break;
        }

        case 5:

            printf("Digite o tipo de percurso: \n");
            printf("    1) Pré-ordem \n");
            printf("    2) In-ordem \n");
            printf("    3) Pós-ordem \n");
            printf("    4) Largura \n");

            scanf("%d", &opcao);

            switch(opcao){
                case 1:
                    bst_imprime_percurso_pre_ordem(raiz);
                    break;
                case 2:
                    bst_imprime_percurso_in_ordem(raiz);
                    break;
                case 3:
                    bst_imprime_percurso_pos_ordem(raiz);
                    break;
                case 4:
                    bst_imprime_percurso_em_largura(raiz);
                    break;
                default:
                    bst_imprime_percurso_in_ordem(raiz);
                    break;
            }

            printf("\n Operacao realizada.\n");

            break;

        case 6:
            chave = bst_calcula_altura(raiz);

            printf("Altura: %d.\n",chave);

            printf("Operacao de remocao realizada.\n");

            break;


        case 7:

            bst_destruir(raiz);

            raiz = NULL;

            printf("Arvore destruida.\n");

            break;

        case 0:

            printf("Encerrando programa.\n");

            break;

        default:

            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    /*
     * Evita vazamento caso o usuário
     * saia sem escolher a opção 5.
     */

    bst_destruir(raiz);

    return EXIT_SUCCESS;
}
