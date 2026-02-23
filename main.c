#include <stdio.h>

#define MAX_CIDADES 100
#define MAX_NOME 30

typedef struct No {
    int id_destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    char nome[MAX_NOME];
    No* lista_adj;
} Cidade;

Cidade grafo[MAX_CIDADES];
int total_cidades = 0;

int buscar_id(char *nome);

int main (int argc, char *argv[]) {
    int n, conjunto = 1;
    while (scanf("%d", &n) == 1) {
        if (conjunto > 1) {
            printf("\n");
        }
        printf("Conjunto #%d\n", conjunto++);
        total_cidades = n;

        for (int i = 0; i < n; i++) {
            scanf("%s", grafo[i].nome);
            grafo[i].lista_adj = NULL;
        }

    }
}

int buscar_id(char *nome) {
    for (int i = 0; i < total_cidades; i++) {
        if (strcmp(grafo[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}
void adicionar_aresta(int origem, int destino, int tempo) {
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->id_destino = destino;
    novo_no->peso = tempo;
    novo_no->prox = grafo[origem].lista_adj;
    grafo[origem].lista_adj = novo_no;
}