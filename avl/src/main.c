#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

enum menu_opcoes
{
    CRIAR_AVL=1,
    INSERIR_AVL=2,
    REMOVER_AVL=3,
    PESQUISAR_AVL=4,
    IMPRIMIR_AVL=5,
    CALC_ALTURA=6,
    DESTRUIR_AVL=7,
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
    printf("      ÁRVORE AVL\n");
    printf("============================\n");
    printf("1 - Criar arvore\n");
    printf("2 - Inserir chave\n");
    printf("3 - Remover chave\n");
    printf("4 - Pesquisar chave\n");
    printf("5 - Imprimir avl\n");
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

            case CRIAR_AVL:

                if (raiz != NULL) {
                    printf("A arvore ja foi criada.\n");
                    break;
                }

                printf("Digite a chave da raiz: ");
                scanf("%d", &chave);

                raiz = avl_criar(chave);

                if (raiz == NULL) {
                    printf("Erro ao criar a arvore.\n");
                }
                else {
                    printf("Arvore criada com sucesso.\n");
                }

                break;

            case INSERIR_AVL:

                printf("Digite a chave para inserir: ");
                scanf("%d", &chave);

                raiz = avl_inserir(raiz, chave);

                printf("Operacao de insercao realizada.\n");

                break;

            case REMOVER_AVL:

                printf("Digite a chave para remover: ");
                scanf("%d", &chave);

                raiz = avl_remover(raiz, chave);

                printf("Operacao de remocao realizada.\n");

                break;

            case PESQUISAR_AVL: {

                printf("Digite a chave para pesquisar: ");
                scanf("%d", &chave);

                no_t *resultado =
                avl_pesquisar(raiz, chave);

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

            case IMPRIMIR_AVL:

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
                            avl_imprime_percurso_pre_ordem(raiz);
                            break;
                        case IN_ORDEM:
                            avl_imprime_percurso_in_ordem(raiz);
                            break;
                        case POS_ORDEM:
                            avl_imprime_percurso_pos_ordem(raiz);
                            break;
                        case LARGURA:
                            avl_imprime_percurso_em_largura(raiz);
                            break;
                        case SAIR:

                            break;
                        default:
                            printf("Opcao invalida.\n");
                            break;
                    }
                }while(opcao != 0);

                opcao = CRIAR_AVL;

                break;

                        case CALC_ALTURA:
                            chave = avl_calcula_altura(raiz);

                            printf("Altura: %d.\n",chave);

                            printf("Operacao de remocao realizada.\n");

                            break;


                        case DESTRUIR_AVL:

                            avl_destruir(raiz);

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

    avl_destruir(raiz);

    return EXIT_SUCCESS;
}
