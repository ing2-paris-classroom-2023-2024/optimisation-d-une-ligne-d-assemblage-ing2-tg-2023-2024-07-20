#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "exclusion.h"

// Définition de la structure Node
struct maillon{
    int sommet;
    struct maillon* next;
};

// Définition de la structure Graph
struct Graph {
    int numSom;
    struct maillon** adjLists;
};

// Structure pour le degré des sommets
struct Degsom {
    int som;
    int degre;
};

// Création d'un nouveau nœud
struct maillon* newm(int v) {
    struct maillon* newN = malloc(sizeof(struct maillon));
    newN->sommet = v;
    newN->next = NULL;
    return newN;
}

// Création d'un graphe
struct Graph* createGraph(int somm) {
    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->numSom = somm;

    graph->adjLists = malloc(somm * sizeof(struct maillon*));
    for (int i = 0; i < somm; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

// Ajout d'une arête au graphe
void addEdge(struct Graph* graph, int src, int dest) {
    src--; // Ajuster pour l'indexation à partir de 0
    dest--;

    struct maillon* nouvm = newm(dest);
    nouvm->next = graph->adjLists[src];
    graph->adjLists[src] = nouvm;

    nouvm = newm(src);
    nouvm->next = graph->adjLists[dest];
    graph->adjLists[dest] = nouvm;
}

// Fonction pour calculer le degré de chaque sommet
void calculateDegrees(struct Graph* graph, struct Degsom  degrees[]) {
    for (int i = 0; i < graph->numSom; i++) {
        struct maillon* temp = graph->adjLists[i];
        degrees[i].som = i;
        degrees[i].degre = 0;
        while (temp) {
            degrees[i].degre++;
            temp = temp->next;
        }
    }
}

// Fonction de comparaison pour le tri
int compareDegrees(const void* a, const void* b) {
    struct Degsom * a1 = (struct Degsom *)a;
    struct Degsom * b1 = (struct Degsom *)b;
    return b1->degre - a1->degre;
}

// Coloriage du graphe selon l'algorithme de Welsh-Powell
void colorGraphWelshPowell(struct Graph* graph) {
    struct Degsom  degrees[graph->numSom];
    calculateDegrees(graph, degrees);

    qsort(degrees, graph->numSom, sizeof(degrees[0]), compareDegrees);

    int color[graph->numSom];
    bool available[graph->numSom];

    for (int i = 0; i < graph->numSom; i++) {
        color[i] = -1;
        available[i] = true;
    }

    for (int i = 0; i < graph->numSom; i++) {
        int u = degrees[i].som;

        struct maillon* t = graph->adjLists[u];
        while (t) {
            if (color[t->sommet] != -1) {
                available[color[t->sommet]] = false;
            }
            t = t->next;
        }

        int cr;
        for (cr = 0; cr < graph->numSom; cr++) {
            if (available[cr]) break;
        }

        color[u] = cr;

        for (int j = 0; j < graph->numSom; j++) {
            available[j] = true;
        }
    }

    for (int u = 0; u < graph->numSom; u++) {
        printf("Le sommet %d est dans la station %d\n", u + 1, color[u] + 1);
    }
}

// Fonction principale
int exclusion(char fichierexclusions[50]) {
    FILE *file = fopen(fichierexclusions, "r");
    if (file == NULL) {
        printf("Erreur à l'ouverture du fichier\n");
        return -1;
    }

    int maxIndex = 0;
    int src, dest;
    while (fscanf(file, "%d %d", &src, &dest) != EOF) {
        if (src > maxIndex) maxIndex = src;
        if (dest > maxIndex) maxIndex = dest;
    }

    struct Graph *graph = createGraph(maxIndex + 1);
    fseek(file, 0, SEEK_SET);

    while (fscanf(file, "%d %d", &src, &dest) != EOF) {
        addEdge(graph, src, dest);
    }

    colorGraphWelshPowell(graph);

    fclose(file);
    return 0;
}