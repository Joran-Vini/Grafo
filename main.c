#include <stdio.h>


typedef struct No {
    int id_destino;
    int peso;
    struct No *prox;
} No;

int main (int argc, char *argv[]) {
    int n, conjunto = 1;
    while (scanf("%d", &n) == 1) {
        if (conjunto > 1) {
            printf("\n");
        }
        
    }
}