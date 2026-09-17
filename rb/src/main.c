#include <stdio.h>
#include <stdlib.h>

#include "rb.h"

enum menu_opcoes
{
    CRIAR_RB=1,
    INSERIR_RB=2,
    REMOVER_RB=3,
    PESQUISAR_RB=4,
    IMPRIMIR_RB=5,
    CALC_ALTURA=6,
    DESTRUIR_RB=7,
    SAIR=0
};

enum submenu_imprimir
{
    PRE_ORDEM=1,
    IN_ORDEM=2,
    POS_ORDEM=3,
    LARGURA=4
};

static void imprimir_menu(void)
{
    printf("\n");
    printf("============================\n");
    printf("      ÁRVORE RB\n");
    printf("============================\n");
    printf("1 - Criar arvore\n");
    printf("2 - Inserir chave\n");
    printf("3 - Remover chave\n");
    printf("4 - Pesquisar chave\n");
    printf("5 - Imprimir rb\n");
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

        case CRIAR_RB:

            if (raiz != NULL) {
                printf("A arvore ja foi criada.\n");
                break;
            }

            printf("Digite a chave da raiz: ");
            scanf("%d", &chave);

            raiz = rb_criar(chave);

            if (raiz == NULL) {
                printf("Erro ao criar a arvore.\n");
            }
            else {
                printf("Arvore criada com sucesso.\n");
            }

            break;

        case INSERIR_RB:

            printf("Digite a chave para inserir: ");
            scanf("%d", &chave);

            raiz = rb_inserir(raiz, chave);

            printf("Operacao de insercao realizada.\n");

            break;

        case REMOVER_RB:

            printf("Digite a chave para remover: ");
            scanf("%d", &chave);

            raiz = rb_remover(raiz, chave);

            printf("Operacao de remocao realizada.\n");

            break;

        case PESQUISAR_RB: {

            printf("Digite a chave para pesquisar: ");
            scanf("%d", &chave);

            no_t *resultado =
                rb_pesquisar(raiz, chave);

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

        case IMPRIMIR_RB:

            do{
                printf("Digite o tipo de percurso: \n");
                printf("    1) Pré-ordem \n");
                printf("    2) In-ordem \n");
                printf("    3) Pós-ordem \n");
                printf("    4) Largura \n");
                printf("    0) Retornar menu principal \n");

                scanf("%d", &opcao);

                switch(opcao){
                    case PRE_ORDEM:
                        rb_imprime_percurso_pre_ordem(raiz);
                        break;
                    case IN_ORDEM:
                        rb_imprime_percurso_in_ordem(raiz);
                        break;
                    case POS_ORDEM:
                        rb_imprime_percurso_pos_ordem(raiz);
                        break;
                    case LARGURA:
                        rb_imprime_percurso_em_largura(raiz);
                        break;
                    case SAIR:

                        break;
                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
            }while(opcao != 0);

            opcao = CRIAR_RB;

            break;

        case CALC_ALTURA:
            chave = rb_calcula_altura(raiz);

            printf("Altura: %d.\n",chave);

            printf("Operacao de remocao realizada.\n");

            break;


        case DESTRUIR_RB:

            rb_destruir(raiz);

            raiz = NULL;

            printf("Arvore destruida.\n");

            break;

        case SAIR:

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

    rb_destruir(raiz);

    return EXIT_SUCCESS;
}
