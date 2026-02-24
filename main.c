#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CIDADES 100
#define MAX_NOME 30

int dist[MAX_CIDADES];
int pai[MAX_CIDADES];

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
void adicionar_voo(int origem, int destino, int tempo);
void liberar_grafo();
void processar_turne(char *artista, int permanencia, char* origem, char* destino);
int djkistra(int origem, int destino);

int main (int argc, char *argv[]) {
    int n, conjunto = 1;
    //Ler número de cidades
    while (scanf("%d", &n) == 1) {
        
        printf("Conjunto #%d\n", conjunto++);
        total_cidades = n;

        // ler cidades
        for (int i = 0; i < n; i++) {
            scanf("%s", grafo[i].nome);
            grafo[i].lista_adj = NULL;
        }
        // ler conexões
        for (int i = 0; i < n; i++) {
            int num_conexoes;
            scanf("%d", &num_conexoes);

            for (int j = 0; j < num_conexoes; j++) {
                char nome_destino[MAX_NOME];
                int tempo;
                scanf("%s %d", nome_destino, &tempo);
                int id_destino = buscar_id(nome_destino);
                adicionar_voo(i, id_destino, tempo);
            }
        }

        // ler artistas
        int m;
        scanf("%d", &m);
        for (int i = 0; i < m; i++) {
            char nome_artista[MAX_NOME], origem[MAX_NOME], destino[MAX_NOME];
            int x_permanencia;
            scanf("%s %d %s %s", nome_artista, &x_permanencia, origem, destino);
            processar_turne(nome_artista, x_permanencia, origem, destino);
        }
        liberar_grafo();
    }
    return 0;
}

int buscar_id(char *nome) {
    for (int i = 0; i < total_cidades; i++) {
        if (strcmp(grafo[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}
void adicionar_voo(int origem, int destino, int tempo) {
    No* novo_no = (No*) malloc(sizeof(No));
    novo_no->id_destino = destino;
    novo_no->peso = tempo;
    novo_no->prox = grafo[origem].lista_adj;
    grafo[origem].lista_adj = novo_no;
}

void liberar_grafo() {
    for (int i = 0; i < total_cidades; i++) {
        No* atual = grafo[i].lista_adj;
        while (atual != NULL) {
            No* prox = atual->prox;
            free(atual);
            atual = prox;
        }
        grafo[i].lista_adj = NULL;
    }
}

void processar_turne(char *artista, int permanencia, char* origem, char* destino) {
    int id_origem = buscar_id(origem);
    int id_destino = buscar_id(destino);

    if (!djkistra(id_origem, id_destino)) {
        printf("%s\nturne cancelada\n\n", artista);
        return;
    }
    //Se chegou aqui é possivel a turne
    printf("%s\n", artista);

    int rota[MAX_CIDADES];
    int conta_rota = 0;
    int aux = id_destino;

    while (aux != -1) {
        rota[conta_rota++] = aux;
        aux = pai[aux];
    }

    //Inverter o array para obter a ordem correta
    for (int i = 0; i < conta_rota / 2; i++) {
        int temp = rota[i];
        rota[i] = rota[conta_rota - 1 - i];
        rota[conta_rota - 1 - i] = temp;
    }

    int dia_chegada[MAX_CIDADES];
    int tempo_atual = 0;
    for (int i = 0; i < conta_rota; i++) {
        dia_chegada[i] = (tempo_atual / 24) + 1;
        tempo_atual += permanencia;

        if (i < conta_rota - 1) {
            int u = rota[i];
            int v = rota[i + 1];

            No* p = grafo[u].lista_adj;
            while (p != NULL) {
                if (p->id_destino == v) {
                    tempo_atual += p->peso;
                    break;
                }
                p = p->prox;
            }
        }
    }

    int ultimo_dia = dia_chegada[conta_rota - 1];
    int idx = 0;

    for (int d = 1; d <= ultimo_dia; d++) {
        printf("%d:", d);

        while (idx <conta_rota && dia_chegada[idx] == d) {
                printf(" %s", grafo[rota[idx]].nome);
                idx++;
            }
        printf("\n");    
        }
       
    printf("%d\n\n", dist[id_destino]);
}

int djkistra(int origem, int destino) {

    int visitado[MAX_CIDADES];
    int cont_cidades[MAX_CIDADES]; 

    for (int i = 0; i < total_cidades; i++) {
        dist[i] = 99999999;
        pai[i] = -1;
        visitado[i] = 0;
        cont_cidades[i] = 0;
    }
    dist[origem] = 0;
    cont_cidades[origem] = 1; 

    for (int i = 0; i < total_cidades; i++) {
        int min_dist = -1;
        for (int j = 0; j < total_cidades; j++) {
            if (!visitado[j] && (dist[j] < dist[min_dist] || min_dist == -1)) {
                min_dist = j;
            }
        }
        if (min_dist == -1) {
            break;
        }
        visitado[min_dist] = 1;

        //Explorar os vizinhos do nó atual
        No* atual = grafo[min_dist].lista_adj;
        while (atual != NULL) {
            int vizinho = atual->id_destino;
            int peso = atual->peso;

            if (dist[min_dist] + peso < dist[vizinho]) {
                dist[vizinho] = dist[min_dist] + peso;
                pai[vizinho] = min_dist;
            }

            //Aplicar regras de desempate 
            
            int novo_tmp = dist[min_dist] + peso;
            int nova_qntd = cont_cidades[min_dist] + 1;
            int atualizar = 0;

            if (novo_tmp < dist[vizinho]) {
                atualizar = 1;
            } else if (novo_tmp == dist[vizinho]) {
                if (nova_qntd < cont_cidades[vizinho]) {
                    atualizar = 1;
                } else if (nova_qntd == cont_cidades[vizinho]) {
                    if (strcmp(grafo[min_dist].nome, grafo[pai[vizinho]].nome) < 0) {
                        atualizar = 1;
                    }
                }
            }
            if (atualizar) {
                dist[vizinho] = novo_tmp;
                pai[vizinho] = min_dist;
                cont_cidades[vizinho] = nova_qntd;
            }

            atual = atual->prox;
        }
    }
    if (dist[destino] == 99999999) {
        return 0; // Não há caminho
    }
    return 1; // Caminho encontrado
}