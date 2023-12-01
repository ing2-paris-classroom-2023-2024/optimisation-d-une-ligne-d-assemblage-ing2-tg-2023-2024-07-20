#include <stdio.h>
#include <stdlib.h>

#define MAX_SOMMETS 100
#define MAX_EXCLUSIONS 100

typedef struct {
    int sommet;
    double temps;
} Sommet;

typedef struct {
    int station;
    int* sommets;
    int nb_sommets;
    int temps_restant;
} Station;

int est_exclus(int sommet1, int sommet2, int** exclusions, int nb_exclusions) {
    for (int i = 0; i < nb_exclusions; i++) {
        if ((exclusions[i][0] == sommet1 && exclusions[i][1] == sommet2) || (exclusions[i][0] == sommet2 && exclusions[i][1] == sommet1)) {
            return 1;
        }
    }
    return 0;
}

int est_predecesseur(int sommet, int* predecesseurs, int nb_predecesseurs) {
    for (int i = 0; i < nb_predecesseurs; i++) {
        if (predecesseurs[i] == sommet) {
            return 1;
        }
    }
    return 0;
}

void regrouper_sommets(Sommet* sommets, int nb_sommets, int T0, int** exclusions, int nb_exclusions) {
    Station* stations = malloc(MAX_SOMMETS * sizeof(Station));
    int nb_stations = 0;
    int t_stations;
    int* predecesseurs = malloc(MAX_SOMMETS * sizeof(int));
    int nb_predecesseurs = 0;

    for (int i = 0; i < nb_sommets; i++) {
        int sommet = sommets[i].sommet;
        int temps = sommets[i].temps;

        if (!est_predecesseur(sommet, predecesseurs, nb_predecesseurs)) {
            int station_existante = 0;
            for (int j = 0; j < nb_stations; j++) {
                if (temps <= stations[j].temps_restant && !est_exclus(sommet, stations[j].sommets[0], exclusions, nb_exclusions))
                {
                    stations[j].sommets[stations[j].nb_sommets] = sommet;
                    stations[j].nb_sommets++;
                    station_existante = 1;
                    stations[j].temps_restant = stations[j].temps_restant - temps;
                    break;
                }
            }
// && temps <= stations[nb_stations].temps_restant
            if (!station_existante) {
                stations[nb_stations].station = nb_stations + 1;
                stations[nb_stations].sommets = malloc(MAX_SOMMETS * sizeof(int));
                stations[nb_stations].sommets[0] = sommet;
                stations[nb_stations].nb_sommets = 1;
                stations[nb_stations].temps_restant = T0-temps;
                nb_stations++;
            }
        }

        predecesseurs[nb_predecesseurs] = sommet;
        nb_predecesseurs++;
    }

    for (int i = 0; i < nb_stations; i++) {
        t_stations = (T0-stations[i].temps_restant);
        printf("Station %d (%d.%02ds): ", stations[i].station, t_stations/100, t_stations%100 );
        for (int j = 0; j < stations[i].nb_sommets; j++) {
            printf("%d ", stations[i].sommets[j]);
        }
        printf("\n");
    }

    free(predecesseurs);
    for (int i = 0; i < nb_stations; i++) {
        free(stations[i].sommets);
    }
    free(stations);
}

int main() {
    FILE* file_precedences = fopen("pred.txt", "r");
    FILE* file_temps = fopen("tpsope.txt", "r");
    //FILE* file_exclusions = fopen("exclusions.txt", "r");

    Sommet* sommets = malloc(MAX_SOMMETS * sizeof(Sommet));
    int nb_sommets = 0;

    int T0;

    int** exclusions = malloc(MAX_EXCLUSIONS * sizeof(int*));
    int nb_exclusions = 0;

    // Lire le fichier precedences.txt
    if (file_precedences != NULL) {
        int sommet1, sommet2;
        while (fscanf(file_precedences, "%d %d", &sommet1, &sommet2) == 2) {
            // Ajouter les sommets à la liste des sommets
            sommets[nb_sommets].sommet = sommet1;
            sommets[nb_sommets].temps = 0.0;
            nb_sommets++;

            sommets[nb_sommets].sommet = sommet2;
            sommets[nb_sommets].temps = 0.0;
            nb_sommets++;
        }
        fclose(file_precedences);
    }

    // Lire le fichier temps.txt
    if (file_temps != NULL) {
        int sommet;
        double temps;
        while (fscanf(file_temps, "%d %lf", &sommet, &temps) == 2) {
            // Mettre à jour le temps des sommets correspondants
            for (int i = 0; i < nb_sommets; i++) {
                if (sommets[i].sommet == sommet) {
                    sommets[i].temps = temps;
                    break;
                }
            }
        }
        fclose(file_temps);
    }


    // Lire le fichier temps_cycle.txt pour obtenir la valeur de T0
    FILE* file_temps_cycle = fopen("temps_cycle.txt", "r");
    if (file_temps_cycle != NULL) {
        fscanf(file_temps_cycle, "%d", &T0);
        fclose(file_temps_cycle);
    }
    printf("temps par cycle : %ds\n\n",T0/100);
    // Regrouper les sommets par station
    regrouper_sommets(sommets, nb_sommets, T0, exclusions, nb_exclusions);

    // Libérer la mémoire allouée
    for (int i = 0; i < nb_exclusions; i++) {
        free(exclusions[i]);
    }
    free(exclusions);
    free(sommets);

    return 0;
}