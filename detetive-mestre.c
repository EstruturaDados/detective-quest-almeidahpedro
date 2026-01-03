#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===================== ESTRUTURAS ===================== */

typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

#define TAM_HASH 10

typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *prox;
} HashNode;

/* ===================== FUNÇÕES DE SALA ===================== */

/*
 * criarSala
 * Cria dinamicamente um cômodo com nome e pista associada
 */
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = nova->dir = NULL;
    return nova;
}

/* ===================== BST DE PISTAS ===================== */

/*
 * inserirPista
 * Insere pista na árvore BST de forma ordenada
 */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (!raiz) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);

    return raiz;
}

void exibirPistas(PistaNode *raiz) {
    if (raiz) {
        exibirPistas(raiz->esq);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

/* ===================== TABELA HASH ===================== */

unsigned int hash(const char *str) {
    unsigned int h = 0;
    while (*str)
        h = h * 31 + *str++;
    return h % TAM_HASH;
}

/*
 * inserirNaHash
 * Associa pista a um suspeito
 */
void inserirNaHash(HashNode *tabela[], const char *pista, const char *suspeito) {
    unsigned int idx = hash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];
    tabela[idx] = novo;
}

/*
 * encontrarSuspeito
 * Retorna o suspeito associado a uma pista
 */
char* encontrarSuspeito(HashNode *tabela[], const char *pista) {
    unsigned int idx = hash(pista);
    HashNode *aux = tabela[idx];

    while (aux) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

/* ===================== EXPLORAÇÃO ===================== */

/*
 * explorarSalas
 * Navega pela mansão e ativa coleta de pistas
 */
void explorarSalas(Sala *atual, PistaNode **bst, HashNode *hashTable[]) {
    char op;

    while (atual) {
        printf("\n📍 Sala atual: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("🔎 Pista encontrada: %s\n", atual->pista);
            *bst = inserirPista(*bst, atual->pista);
            atual->pista[0] = '\0';
        }

        printf("\n[e] esquerda | [d] direita | [s] sair: ");
        scanf(" %c", &op);

        if (op == 'e' && atual->esq)
            atual = atual->esq;
        else if (op == 'd' && atual->dir)
            atual = atual->dir;
        else if (op == 's')
            break;
        else
            printf("Caminho inválido.\n");
    }
}

/* ===================== JULGAMENTO ===================== */

/*
 * verificarSuspeitoFinal
 * Avalia se existem pelo menos 2 pistas contra o suspeito
 */
void contarPistas(PistaNode *raiz, HashNode *hashTable[], const char *acusado, int *contador) {
    if (raiz) {
        contarPistas(raiz->esq, hashTable, acusado, contador);

        char *suspeito = encontrarSuspeito(hashTable, raiz->pista);
        if (suspeito && strcmp(suspeito, acusado) == 0)
            (*contador)++;

        contarPistas(raiz->dir, hashTable, acusado, contador);
    }
}

void verificarSuspeitoFinal(PistaNode *bst, HashNode *hashTable[]) {
    char acusado[50];
    int total = 0;

    printf("\n👤 Quem você acusa? ");
    scanf(" %[^\n]", acusado);

    contarPistas(bst, hashTable, acusado, &total);

    if (total >= 2)
        printf("\n✅ %s foi considerado CULPADO (%d pistas).\n", acusado, total);
    else
        printf("\n❌ Provas insuficientes contra %s (%d pista(s)).\n", acusado, total);
}

/* ===================== MAIN ===================== */

int main() {
    HashNode *hashTable[TAM_HASH] = {NULL};
    PistaNode *bst = NULL;

    // Salas
    Sala *hall = criarSala("Hall", "Pegadas no tapete");
    Sala *cozinha = criarSala("Cozinha", "Faca suja");
    Sala *sala = criarSala("Sala de Estar", "Relógio quebrado");
    Sala *biblioteca = criarSala("Biblioteca", "Livro rasgado");
    Sala *jardim = criarSala("Jardim", "Terra remexida");

    // Mapa
    hall->esq = sala;
    hall->dir = cozinha;
    sala->esq = biblioteca;
    sala->dir = jardim;

    // Hash: pista → suspeito
    inserirNaHash(hashTable, "Pegadas no tapete", "Sr. Black");
    inserirNaHash(hashTable, "Faca suja", "Sra. White");
    inserirNaHash(hashTable, "Relógio quebrado", "Sr. Black");
    inserirNaHash(hashTable, "Livro rasgado", "Sr. Green");
    inserirNaHash(hashTable, "Terra remexida", "Sr. Black");

    // Exploração
    explorarSalas(hall, &bst, hashTable);

    // Relatório
    printf("\n📋 Pistas coletadas:\n");
    exibirPistas(bst);

    // Julgamento
    verificarSuspeitoFinal(bst, hashTable);

    return 0;
}
