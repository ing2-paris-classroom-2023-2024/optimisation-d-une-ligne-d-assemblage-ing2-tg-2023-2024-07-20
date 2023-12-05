#include <stdio.h>
#include <stdlib.h>
#include "exclusion.h"
#include "precedences_et_cycle.h"
#include "exclusions_et_precedences.h"
#include "exclusions_et_cycle.h"



#define MAX_SOMMETS 100
#define MAX_EXCLUSIONS 100

//test
typedef struct {
    int sommet;
    double temps;
    int positionne;
    int nb_predecesseurs;
    int* predecesseurs;
} Sommet;

typedef struct {
    int station;
    int* sommets;
    int nb_sommets;
    int temps_restant;
} Station;

// Structure pour représenter une paire d'opérations interdites



int est_exclus(int sommet1, int sommet2, int** exclusions, int nb_exclusions) {
    for (int i = 0; i < nb_exclusions; i++) {
        if ((exclusions[i][0] == sommet1 && exclusions[i][1] == sommet2) || (exclusions[i][0] == sommet2 && exclusions[i][1] == sommet1)) {
            return 1;
        }
    }
    return 0;
}

int est_predecesseur(Sommet* sommet, int id, int nb_stations, Station* stations)
{
    // verifier qu'un sommet n'est pas prédécesseur d'un sommet qui n'est pas dans la station en cours ou les précédentes
    int nb_trouve=0;
    for (int i = 0; i < nb_stations; i++){ // on percourt les stations existantes
        for (int j=0; j < stations[i].nb_sommets; j++){ // on parcourt les sommets de chaque stations
            for (int k=0; k < sommet[id].nb_predecesseurs;k++){ // on parcourt les predecesseurs du sommet recherché
                if (stations[i].sommets[j] == sommet[id].predecesseurs[k])  {
                    nb_trouve++;
                }
            }
        }
    }
    if (sommet[id].nb_predecesseurs==nb_trouve){
        return 0;
    }
    return 1;
}

void regrouper_sommets(Sommet* sommets, int nb_sommets, int T0, int** exclusions, int nb_exclusions) {
    Station* stations = malloc(MAX_SOMMETS * sizeof(Station));
    int nb_stations = 0;
    int nb_sommets_positionnes = 0;
    int positionne_ce_tour = 0;
    int i = 0;

    while (nb_sommets_positionnes < nb_sommets) {
        if (i > nb_sommets) {
            if (positionne_ce_tour == 0) {
                // Si un tour à vide, on ajoute une station
                stations[nb_stations].station = nb_stations + 1;
                stations[nb_stations].sommets = malloc(MAX_SOMMETS * sizeof(int));
                stations[nb_stations].nb_sommets = 0;
                stations[nb_stations].temps_restant = T0;
                nb_stations++;
            }
            positionne_ce_tour = 0;
            i = 0;
        }

        while (sommets[i].positionne == 1) {
            i++;
        }

        int sommet = sommets[i].sommet;
        int temps = sommets[i].temps;

        for (int j = 0; j < nb_stations && i > 0; j++) {
            if (!est_predecesseur(sommets, i, j + 1, stations)) {
                if (temps <= stations[j].temps_restant && sommet > 0) {
                    int exclu = 0;

                    for (int k = 0; k < stations[j].nb_sommets; k++) {
                        if (est_exclus(sommet, stations[j].sommets[k], exclusions, nb_exclusions)) {
                            exclu = 1;
                            break;
                        }
                    }

                    if (!exclu) {
                        positionne_ce_tour++;
                        nb_sommets_positionnes++;
                        stations[j].sommets[stations[j].nb_sommets] = sommet;
                        sommets[i].positionne = 1;
                        stations[j].nb_sommets++;
                        stations[j].temps_restant = stations[j].temps_restant - temps;

                        //printf("Sommet %d positionne dans la station %d\n", sommet, stations[j].station);

                        break;
                    }
                }
            }
            if (sommets[i].positionne == 1) break;
        }

        if (i == 0 && positionne_ce_tour == 0 && sommets[i].positionne == 0) {
            // Cas particulier de la première itération : on crée la première station
            stations[0].station = 1;
            stations[0].sommets = malloc(MAX_SOMMETS * sizeof(int));
            stations[0].sommets[0] = sommet;
            stations[0].nb_sommets = 1;
            stations[0].temps_restant = T0 - temps;
            sommets[0].positionne = 1;
            nb_sommets_positionnes++;
            nb_stations++;

            //printf("Sommet %d positionne dans la station %d\n", sommet, stations[0].station);
        }

        i++;
    }

    printf("Affichage lignes d'assemblages avec toutes les contraintes\n");
    for (int i = 0; i < nb_stations; i++) {
        int t_stations = (T0 - stations[i].temps_restant);
        printf("Station %d (%d.%02ds): ", stations[i].station, t_stations / 100, t_stations % 100);
        printf("\n");

        printf("     ________ \n");
        printf("  ___//_||_\\___");

        // Partie supérieure de la voiture (toit)
        //printf(" |");
        for (int j = 0; j < stations[i].nb_sommets -4; j++) {
            printf("__");
        }
        printf("\n");

        // Affichage des nombres dans la voiture
        printf(" |");
        for (int j = 0; j < stations[i].nb_sommets; j++) {
            printf("%d ", stations[i].sommets[j]);
        }
        printf(" |\n");

        // Partie inférieure de la voiture
        printf(" |");
        for (int j = 0; j < stations[i].nb_sommets; j++) {
            printf("_ _");
        }
        printf("|\n");

        printf("     OO   OO    \n");
        printf("\n");
    }

    // Libération de la mémoire allouée
    for (int i = 0; i < nb_stations; i++) {
        free(stations[i].sommets);
    }
    free(stations);
}


void afficher_menu() {
      // Clear the screen for better visibility
    printf("\n***********************************************************************************************\n");
    printf("******************************      Menu Principal      ******************************************\n");
    printf("**************************************************************************************************\n");
    printf("***      1. Afficher la ligne d'assemblage, toutes contraintes respectees                      ***\n");
    printf("***      2. Afficher la ligne d'assemblage avec les contraintes de precedences et de cycle     ***\n");
    printf("***      3. Afficher le ligne d'assemblage avec la contraintes d'exclusion                     ***\n");
    printf("***      4. Afficher la ligne d'assemblage avec les contraintes d'exclusions et precedences    ***\n");
    printf("***      5. Afficher la ligne d'assemblage avec les contraintes d'exclusions et cycles         ***\n");
    printf("***      6. Quitter                                                                            ***\n");
    printf("**************************************************************************************************\n");

}

int main() {
    char file_temps_name[50];
    char file_precedences_name[50];
    char file_exclusions_name[50];
    char file_temps_cycle_nom[50];

    printf("Entrez le nom du fichier temps (ex: temps.txt) : ");
    scanf("%s", file_temps_name);

    printf("Entrez le nom du fichier precedences (ex: pred.txt) : ");
    scanf("%s", file_precedences_name);

    printf("Entrez le nom du fichier exclusions (ex: exclusions.txt): ");
    scanf("%s", file_exclusions_name);

    printf("Entrez le nom du fichier temps_cycle (ex: temps_cycle.txt) : ");
    scanf("%s", file_temps_cycle_nom);

    FILE* file_temps = fopen(file_temps_name, "r");
    FILE* file_precedences = fopen(file_precedences_name, "r");
    FILE* file_exclusions = fopen(file_exclusions_name, "r");
    FILE* file_temps_cycle = fopen(file_temps_cycle_nom, "r");

    Sommet* sommets = malloc(MAX_SOMMETS * sizeof(Sommet));
    int nb_sommets = 0;

    int T0;
    int choix;

    int** exclusions = malloc(MAX_EXCLUSIONS * sizeof(int*));
    int nb_exclusions = 0;

    // Lire le fichier temps.txt
    if (file_temps != NULL)
    {
        int sommet;
        double temps;
        while (fscanf(file_temps, "%d %lf", &sommet, &temps) == 2)
        {
            // Mettre à jour le temps des sommets correspondants
            sommets[nb_sommets].sommet=sommet;
            sommets[nb_sommets].temps=temps;
            sommets[nb_sommets].positionne=0;
            sommets[nb_sommets].nb_predecesseurs = 0;
            sommets[nb_sommets].predecesseurs = malloc(sommets[nb_sommets].nb_predecesseurs * sizeof(int)); // Allocation de mémoire pour le tableau
            nb_sommets++;

        }
        fclose(file_temps);
    }

    // Lire le fichier precedences.txt
    if (file_precedences != NULL) {
        int sommet1, sommet2;
        while (fscanf(file_precedences, "%d %d", &sommet1, &sommet2) == 2)
        {
            // Ajouter les predecesseurs du sommet en cours
            for (int i = 0; i < nb_sommets; i++)
            {
                if (sommets[i].sommet == sommet2)
                {
                    sommets[i].predecesseurs[sommets[i].nb_predecesseurs]=sommet1;
                    sommets[i].nb_predecesseurs++;
                    break;
                }
            }
        }
        fclose(file_precedences);
    }

    // Lire le fichier exclusions.txt
    if (file_exclusions != NULL) {
        int sommet1, sommet2;
        while (fscanf(file_exclusions, "%d %d", &sommet1, &sommet2) == 2) {
            // Ajouter les exclusions à la liste des exclusions
            exclusions[nb_exclusions] = malloc(2 * sizeof(int));
            exclusions[nb_exclusions][0] = sommet1;
            exclusions[nb_exclusions][1] = sommet2;
            nb_exclusions++;
        }
        fclose(file_exclusions);
    }

    if (file_temps_cycle != NULL) {
        fscanf(file_temps_cycle, "%d", &T0);
        fclose(file_temps_cycle);
    }
    printf("temps par cycle : %ds\n\n",T0/100);
    // Regrouper les sommets par station



    // Libérer la mémoire allouée
    for (int i = 0; i < nb_exclusions; i++) {
        free(exclusions[i]);
    }
    do
    {
        afficher_menu();
        printf("Choix : ");
        scanf("%d", &choix);

            switch (choix) {
                case 1:
                    regrouper_sommets(sommets, nb_sommets, T0, exclusions, nb_exclusions);
                    break;
                case 2:
                    precedences_et_cycle(file_precedences_name, file_temps_cycle_nom,file_temps_name,file_exclusions_name);
                    break;
                case 3:
                    exclusion(file_exclusions_name);
                    break;
                case 4:
                    exclusions_et_precedences(file_precedences_name,file_temps_cycle_nom,file_temps_name,file_exclusions_name);
                    break;
                case 5:
                    exclusions_et_cycle(file_precedences_name, file_temps_cycle_nom,file_temps_name,file_exclusions_name);
                    break;
                case 6:
                    printf("Au revoir!\n");
                    return 0;
                    break;
                default:
                    printf("Choix invalide. Veuillez saisir un nombre entre 1 et 6.\n");
            }
            // Demander à l'utilisateur s'il veut rester dans le même fichier
            do{
                printf("Voulez-vous revenir au menu?(o/n): ");
                scanf(" %c", &choix);
            }while (choix != 'o' && choix != 'n' && choix != 'O' && choix != 'N');

        } while (choix == 'o' || choix == 'O');
    printf("Au revoir!\n");

    free(exclusions);
    free(sommets);
    fclose(file_temps);
    fclose(file_precedences);
    fclose(file_exclusions);
    fclose(file_temps_cycle);




    return 0;
}