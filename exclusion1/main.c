#include <stdio.h>

#include <stdlib.h>

// Structure pour représenter une paire d'opérations interdites
typedef struct {
    int operation1;
    int operation2;
} Exclusion;

// Fonction pour lire les contraintes d'exclusion à partir d'un fichier
Exclusion* lecture(int* nombreContraintes) {

    char nom[50];
    printf("Entrez le nom du fichier (avec le .txt): \n");
    scanf("%s",nom);

    FILE* ifs = fopen(nom, "r");
    if (!ifs) {
        printf( "Erreur ouverture du fichier\n");
        exit(-1);
    }

    int tailleTableau = 20;

    Exclusion* contraintes = malloc(tailleTableau * sizeof(Exclusion));

    int nbContraintes = 0;

    while (fscanf(ifs, "%d %d", &contraintes[nbContraintes].operation1, &contraintes[nbContraintes].operation2) == 2) {
        nbContraintes++;

        if (nbContraintes == tailleTableau) {
            tailleTableau *= 2;
            contraintes = realloc(contraintes, tailleTableau * sizeof(Exclusion));
        }
    }

    fclose(ifs);

    printf("Liste des contraintes:\n");

    for(int i =0; i<nbContraintes;i++)
    {
        printf("%d %d \n",contraintes[i].operation1, contraintes[i].operation2);
    }

    *nombreContraintes = nbContraintes;
    return contraintes;
}



// Fonction pour vérifier si une paire d'opérations est autorisée sur une station
int estAutoriseeSurStation(int station, int* affectations, Exclusion* contraintes, int nombreContraintes, int operation) {
    for (int i = 0; i < nombreContraintes; i++) {
        if ((contraintes[i].operation1 == operation || contraintes[i].operation2 == operation) &&
            affectations[contraintes[i].operation1 - 1] == station && affectations[contraintes[i].operation2 - 1] == station) {
            return 0; // La paire d'opérations interdites est affectée à la même station
        }
    }
    return 1; // La paire d'opérations est autorisée sur la station
}

// Fonction pour répartir les opérations sur les stations en respectant les contraintes
void repartirOperations(int nombreOperations, int nombreStations, int nombreContraintes, Exclusion* contraintes) {

    int* affectations = calloc(nombreOperations, sizeof(int));

    for (int i = 0; i < nombreOperations; i++) {
        for (int j = 1; j <= nombreStations; j++) {
            if (estAutoriseeSurStation(j, affectations, contraintes, nombreContraintes, i + 1)==1) {
                affectations[i] = j;
                break;
            }
        }
    }

    // Affichage des affectations
    printf("\nRepartition des operations:\n");
    for (int i = 0; i < nombreOperations; i++) {
        printf("Operation %d affectee a la station %d\n", i + 1, affectations[i]);
    }

    free(affectations);
}





int main() {

    int nombreContraintes;
    Exclusion* contraintes = lecture( &nombreContraintes);

    int nombreOperations = 35;  // Nombre total d'opérations
    int nombreStations = 20;    // Nombre total de stations

    repartirOperations(nombreOperations, nombreStations, nombreContraintes, contraintes);

    // Libération de la mémoire
    free(contraintes);

    return EXIT_SUCCESS;
}