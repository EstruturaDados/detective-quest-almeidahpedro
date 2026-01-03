#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Estrutura que representa uma sala da mansão.
 * Cada sala possui um nome e dois caminhos:
 * esquerda e direita.
 */
typedef struct Sala {
    char nome[50];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/*
 * criarSala
 * Cria dinamicamente uma sala com o nome informado.
 */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*) malloc(sizeof(Sala));

    if (novaSala == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    strcpy(novaSala->nome, nome);
    novaSala->esq = NULL;
    novaSala->dir = NULL;

    return novaSala;
}

/*
 * explorarSalas
 * Permite ao jogador navegar pela mansão,
 * escolhendo esquerda (e), direita (d) ou sair (s).
 */
void explorarSalas(Sala *atual) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Verifica se é um nó-folha
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Esta sala não possui mais caminhos.\n");
            printf("Fim da exploração.\n");
            break;
        }

        printf("Escolha um caminho:\n");
        printf("e - esquerda\n");
        printf("d - direita\n");
        printf("s - sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            if (atual->esq != NULL) {
                atual = atual->esq;
            } else {
                printf("Não há caminho à esquerda.\n");
            }
        } 
        else if (opcao == 'd') {
            if (atual->dir != NULL) {
                atual = atual->dir;
            } else {
                printf("Não há caminho à direita.\n");
            }
        } 
        else if (opcao == 's') {
            printf("Saindo da mansão...\n");
            break;
        } 
        else {
            printf("Opção inválida. Tente novamente.\n");
        }
    }
}

/*
 * main
 * Monta manualmente o mapa da mansão
 * e inicia a exploração pelo Hall de Entrada.
 */
int main() {
    // Criação das salas
    Sala *hall = criarSala("Hall de Entrada");
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *cozinha = criarSala("Cozinha");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *jardim = criarSala("Jardim");

    // Montagem da árvore (mapa da mansão)
    hall->esq = salaEstar;
    hall->dir = cozinha;

    salaEstar->esq = biblioteca;
    salaEstar->dir = jardim;

    // Início da exploração
    explorarSalas(hall);

    return 0;
}
