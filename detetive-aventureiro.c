#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura que representa uma sala da mansão (árvore binária)
 */
typedef struct Sala {
    char nome[50];
    char pista[100];      // Pista associada à sala (vazia se não houver)
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/*
 * Estrutura que representa um nó da árvore BST de pistas
 */
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/*
 * criarSala
 * Cria dinamicamente uma sala com nome e pista (opcional)
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);

    if (pista != NULL)
        strcpy(novaSala->pista, pista);
    else
        novaSala->pista[0] = '\0';

    novaSala->esq = NULL;
    novaSala->dir = NULL;

    return novaSala;
}

/*
 * inserirPista
 * Insere uma pista na árvore BST de forma ordenada
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

/*
 * explorarSalasComPistas
 * Permite a navegação pela mansão e coleta automática de pistas
 */
void explorarSalasComPistas(Sala *atual, PistaNode **pistasColetadas) {
    char opcao;

    while (atual != NULL) {
        printf("\n📍 Você está na sala: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
            atual->pista[0] = '\0'; // Evita coletar novamente
        }

        printf("\nEscolha uma opção:\n");
        printf("e - ir para a esquerda\n");
        printf("d - ir para a direita\n");
        printf("s - sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            if (atual->esq != NULL)
                atual = atual->esq;
            else
                printf("Não há caminho à esquerda.\n");
        }
        else if (opcao == 'd') {
            if (atual->dir != NULL)
                atual = atual->dir;
            else
                printf("Não há caminho à direita.\n");
        }
        else if (opcao == 's') {
            printf("\nEncerrando a exploração...\n");
            break;
        }
        else {
            printf("Opção inválida.\n");
        }
    }
}

/*
 * exibirPistas
 * Exibe todas as pistas em ordem alfabética (percurso em ordem)
 */
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

/*
 * main
 * Monta o mapa da mansão e inicia o jogo
 */
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada", "Pegadas recentes no tapete");
    Sala *salaEstar = criarSala("Sala de Estar", "Almofada fora do lugar");
    Sala *cozinha = criarSala("Cozinha", "Faca com manchas suspeitas");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado na estante");
    Sala *jardim = criarSala("Jardim", "Terra revirada perto da árvore");

    // Montagem do mapa (árvore binária)
    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = biblioteca;
    salaEstar->dir = jardim;

    // Árvore de pistas (BST)
    PistaNode *pistasColetadas = NULL;

    // Início da exploração
    explorarSalasComPistas(hall, &pistasColetadas);

    // Exibição final das pistas
    printf("\n📋 Pistas coletadas (ordem alfabética):\n");
    exibirPistas(pistasColetadas);

    return 0;
}
