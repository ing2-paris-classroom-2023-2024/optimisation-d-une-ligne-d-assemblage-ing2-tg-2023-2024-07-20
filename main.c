#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPERATIONS 100

// Structure pour représenter un graphe
typedef struct {
    int nombreOperations;
    char operations[MAX_OPERATIONS][50];
    int matriceAdjacence[MAX_OPERATIONS][MAX_OPERATIONS];
} Graphe;

// Initialiser le graphe
void initialiserGraphe(Graphe *graphe) {
    int i, j;

    // Initialiser la matrice d'adjacence à zéro
    for (i = 0; i < MAX_OPERATIONS; i++) {
        for (j = 0; j < MAX_OPERATIONS; j++) {
            graphe->matriceAdjacence[i][j] = 0;
        }
    }

    // Initialiser le nombre d'opérations à zéro
    graphe->nombreOperations = 0;
}

// Ajouter une relation de précédence entre deux opérations
void ajouterRelation(Graphe *graphe, char operation1[], char operation2[]) {
    int i, index1 = -1, index2 = -1;

    // Rechercher les indices des opérations dans la liste
    for (i = 0; i < graphe->nombreOperations; i++) {
        if (strcmp(graphe->operations[i], operation1) == 0) {
            index1 = i;
        }
        if (strcmp(graphe->operations[i], operation2) == 0) {
            index2 = i;
        }
    }

    // Si les opérations ne sont pas trouvées, les ajouter à la liste
    if (index1 == -1) {
        strcpy(graphe->operations[graphe->nombreOperations], operation1);
        index1 = graphe->nombreOperations++;
    }
    if (index2 == -1) {
        strcpy(graphe->operations[graphe->nombreOperations], operation2);
        index2 = graphe->nombreOperations++;
    }

    // Ajouter l'arc dans la matrice d'adjacence
    graphe->matriceAdjacence[index1][index2] = 1;
}

// Fonction récursive pour le DFS
void dfs(Graphe *graphe, int sommet, int visite[]) {
    int i;

    printf("%s ", graphe->operations[sommet]);
    visite[sommet] = 1;

    // Parcourir les voisins non visités
    for (i = 0; i < graphe->nombreOperations; i++) {
        if (graphe->matriceAdjacence[sommet][i] == 1 && !visite[i]) {
            dfs(graphe, i, visite);
        }
    }
}

// Fonction principale pour le parcours DFS
void parcourirGraphe(Graphe *graphe) {
    int i, visite[MAX_OPERATIONS];

    // Initialiser le tableau de visite à zéro
    for (i = 0; i < MAX_OPERATIONS; i++) {
        visite[i] = 0;
    }

    // Appliquer le DFS à partir de chaque sommet non visité
    for (i = 0; i < graphe->nombreOperations; i++) {
        if (!visite[i]) {
            dfs(graphe, i, visite);
        }
    }

    printf("\n");
}

int main() {
    Graphe graphe;
    FILE *fichier;
    char nomFichier[100];
    char operation1[50], operation2[50];

    // Initialiser le graphe
    initialiserGraphe(&graphe);

    // Demander à l'utilisateur le nom du fichier
    printf("Entrez le nom du fichier (precedences.txt) : ");
    scanf("%s", nomFichier);

    // Ouvrir le fichier
    fichier = fopen(nomFichier, "r");

    // Lire les relations de précédence à partir du fichier
    while (fscanf(fichier, "%s %s", operation1, operation2) == 2) {
        ajouterRelation(&graphe, operation1, operation2);
    }

    // Fermer le fichier
    fclose(fichier);

    // Parcourir le graphe en utilisant DFS
    printf("Chemin parcourant toutes les operations :\n");
    parcourirGraphe(&graphe);

    return 0;
}