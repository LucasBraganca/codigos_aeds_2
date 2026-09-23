#include "patricia.h"

#include <stdio.h>
#include <stdlib.h>

enum menu_opcoes {
    CRIAR_PATRICIA = 1,
    INSERIR_CHAVE,
    REMOVER_CHAVE,
    BUSCAR_CHAVE,
    IMPRIMIR_PATRICIA,
    MOSTRAR_TAMANHO,
    VALIDAR_PATRICIA,
    DESTRUIR_PATRICIA,
    SAIR = 0
};

static void imprimir_menu(void)
{
    printf("\n");
    printf("================================\n");
    printf("        ARVORE PATRICIA\n");
    printf("================================\n");
    printf("1 - Criar arvore\n");
    printf("2 - Inserir chave\n");
    printf("3 - Remover chave (extensao)\n");
    printf("4 - Buscar chave\n");
    printf("5 - Imprimir arvore\n");
    printf("6 - Mostrar tamanho\n");
    printf("7 - Validar estrutura\n");
    printf("8 - Destruir arvore\n");
    printf("0 - Sair\n");
    printf("================================\n");
    printf("Opcao: ");
}

static void descartar_linha(void)
{
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

static int ler_inteiro(int *valor)
{
    if (scanf("%d", valor) != 1) {
        descartar_linha();
        return 0;
    }
    return 1;
}

static int ler_chave(chave_t *chave)
{
    printf("Digite a chave em decimal (D = %d bits): ", D);
    if (scanf("%u", chave) != 1) {
        descartar_linha();
        printf("Chave invalida.\n");
        return 0;
    }
    return 1;
}

int main(void)
{
    patricia_t *arvore = NULL;
    int opcao = SAIR;

    do {
        imprimir_menu();
        if (!ler_inteiro(&opcao)) {
            printf("Opcao invalida.\n");
            opcao = -1;
            continue;
        }

        switch (opcao) {
        case CRIAR_PATRICIA:
            if (arvore != NULL) {
                printf("A arvore ja foi criada.\n");
                break;
            }
            arvore = patricia_criar();
            if (arvore == NULL)
                printf("Erro ao criar a arvore.\n");
            else
                printf("Arvore criada com sucesso.\n");
            break;

        case INSERIR_CHAVE: {
            chave_t chave;
            if (arvore == NULL) {
                printf("Crie a arvore antes de inserir.\n");
                break;
            }
            if (!ler_chave(&chave))
                break;
            int resultado = patricia_inserir(arvore, chave);
            if (resultado == 1)
                printf("Chave inserida com sucesso.\n");
            else if (resultado == 0)
                printf("A chave ja existe na arvore.\n");
            else
                printf("Nao foi possivel inserir a chave. Verifique a faixa de D bits.\n");
            break;
        }

        case REMOVER_CHAVE: {
            chave_t chave;
            if (arvore == NULL) {
                printf("Crie a arvore antes de remover.\n");
                break;
            }
            if (!ler_chave(&chave))
                break;
            int resultado = patricia_remover(arvore, chave);
            if (resultado == 1)
                printf("Chave removida com sucesso.\n");
            else if (resultado == 0)
                printf("Chave nao encontrada.\n");
            else
                printf("Remocao ainda nao implementada: extensao opcional.\n");
            break;
        }

        case BUSCAR_CHAVE: {
            chave_t chave;
            if (arvore == NULL) {
                printf("Crie a arvore antes de buscar.\n");
                break;
            }
            if (!ler_chave(&chave))
                break;
            if (patricia_buscar(arvore, chave))
                printf("Chave %u encontrada.\n", chave);
            else
                printf("Chave %u nao encontrada.\n", chave);
            break;
        }

        case IMPRIMIR_PATRICIA:
            if (arvore == NULL)
                printf("A arvore ainda nao foi criada.\n");
            else
                patricia_imprimir(arvore);
            break;

        case MOSTRAR_TAMANHO:
            if (arvore == NULL)
                printf("A arvore ainda nao foi criada.\n");
            else
                printf("Quantidade de chaves: %zu.\n", patricia_tamanho(arvore));
            break;

        case VALIDAR_PATRICIA:
            if (arvore == NULL)
                printf("A arvore ainda nao foi criada.\n");
            else if (patricia_validar(arvore))
                printf("A estrutura da Patricia e valida.\n");
            else
                printf("A estrutura da Patricia e invalida.\n");
            break;

        case DESTRUIR_PATRICIA:
            if (arvore == NULL) {
                printf("A arvore ainda nao foi criada.\n");
                break;
            }
            patricia_destruir(arvore);
            arvore = NULL;
            printf("Arvore destruida.\n");
            break;

        case SAIR:
            printf("Encerrando o programa.\n");
            break;

        default:
            printf("Opcao invalida.\n");
            break;
        }
    } while (opcao != SAIR);

    patricia_destruir(arvore);
    return EXIT_SUCCESS;
}
