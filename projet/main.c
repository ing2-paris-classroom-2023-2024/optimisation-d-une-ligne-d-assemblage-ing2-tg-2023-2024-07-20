#include <stdio.h>

#include <stdlib.h>

// Structure pour représenter une paire d'opérations interdites
struct point{
    int val;
    int indice;
};

typedef struct point point;

struct arette{
    point op1;
    point op2;
};


typedef struct arette arette;
typedef struct graphe {
    arette* sommet;
    int ordre;
    int taille;
    int* liste;
    int** adj;
}graphe;

//tableau des sommets

void tri(int* deg, int max, int* liste) //tri decroissant des degre des sommet par ordre décroissant
{
    int temp;
    int temp1;
    for(int i = 0; i< max-1; i++){
        for (int j =0; j < max -i-1; j++){
            if(deg[j] < deg[j+1]){
                temp = deg[j];
                deg[j] = deg[j+1];
                deg[j+1] = temp;

                temp1 = liste[j];
                liste[j] = liste[j+1];
                liste[j+1] = temp1;
            }
        }

    }
}


graphe* Graphe(FILE* ifs,graphe* g){ //creation du graphe
    int op1, op2;
//lecture nombre de lignes
    int c;
    int nbLigne = 1;
    while((c= fgetc(ifs)) != EOF){
        if (c=='\n'){
            nbLigne++;
        }
    }

    printf("ORDRE %d\n",g->ordre);
    g->taille = nbLigne;
    printf("TAILLE (NOMBRE DE CONTRAINES) %d\n",g->taille);
    g->sommet = (arette*)malloc(g->taille*sizeof(arette));
    g-> liste = malloc (sizeof(int) * g->ordre);

    //retour au debut du texte
    fseek(ifs, 0, SEEK_SET);

    int nbs =0; // compteur nombre de sommet

    //lecture de chaque contraintes
    for (int i=0; i<g->taille; ++i) {
        int v1 = 0; //valeur lors du parcours de liste 1 si op1 deja là
        int v2 = 0; //valeur lors du parcours de liste 1 si op2 deja là
        // créer les arêtes du graphe
        fscanf(ifs, "%d%d", &op1, &op2);

        g->sommet[i].op1.val = op1;
        g->sommet[i].op2.val = op2;

        if (nbs >= 1) {
            for (int j = 0; j < nbs; j++) {
                if (g->liste[j] == op1) //si op1 deja dans la liste
                {
                    v1 = 1;
                }

                if (g->liste[j] == op2) //si op2 deja dans la liste
                {
                    v2 = 1;
                }
            }

            if (v1 == 0) {
                g->liste[nbs] = op1;
                nbs++;
            }

            if (v2 == 0) {
                g->liste[nbs] = op2;
                nbs++;
            }

        }

        else { //nbs ==0
            g->liste[nbs] = op1;
            nbs++;
            g->liste[nbs] = op2;
            nbs++;
        }
    }

    

    int s; //sommet temporaire
    int* deg = malloc(sizeof(int) * g->ordre); //tableau des dégré des sommets

    for (int k=0; k < g->ordre; k++){

        int f =0; //valeur sommet relié au sommet 0 ou non
        s = g->liste[k];
        int cpt = 0; //compte les degre sur sommet s

        for (int j = 0; j < g->taille; j++) {
            if (g->sommet[j].op1.val == s) //une liaion
            {
                cpt ++;
            }

            if (g->sommet[j].op2.val == s) //une liaion
            {
                cpt ++;
            }

            if (g->sommet[j].op2.val == 0 && g->sommet[j].op1.val == s )
            {
                deg[k] = 0;
                f = 1;
            }

            if (g->sommet[j].op2.val == s && g->sommet[j].op1.val == 0 ) //une liaion
            {
                deg[k] = 0;
                f = 1;
            }

            if (s == 0 ) //somme inutile
            {
                f = 1;
            }


        }

        if(f== 0) {
            deg[k] = cpt;
        }
        else{
            deg[k] =0;
        }

    }

    tri(deg,g->ordre,g->liste);

/////////////////Afichage des degre
    for (int i=0; i<g->ordre; i++) {
        if (g->liste[i] == 0) {
            printf("");
        }

        else {
            printf("DEGRE DU SOMMET %d: %d\n", g->liste[i], deg[i]);
        }
    }
/////////////////////
    if(g->taille > g->ordre) {

        for (int i = 0; i < g->ordre; i++) {
            for (int j = 0; j < g->ordre; j++) {
                if (g->sommet[j].op1.val == g->liste[i]) {
                    g->sommet[j].op1.indice = i;
                }

                if (g->sommet[j].op2.val == g->liste[i]) {
                    g->sommet[j].op2.indice = i;
                }
            }

        }

    }

    else{

        for (int i = 0; i < g->ordre; i++) {
            for (int j = 0; j < g->taille; j++) {
                if (g->sommet[j].op1.val == g->liste[i]) {
                    g->sommet[j].op1.indice = i;
                }

                if (g->sommet[j].op2.val == g->liste[i]) {
                    g->sommet[j].op2.indice = i;
                }
            }

        }

    }


    //remplir matrice


    g->adj = malloc(g->ordre*sizeof(int*)); //matrice d'adjacence
    for (int i =0; i< g->ordre; i++) {
        g->adj[i] = calloc(g->ordre, sizeof(int));

        if (!g->adj[i]) {
            perror("Allocation de mémoire pour la matrice d'adjacence a échoué");
            exit(EXIT_FAILURE);
        }

    }


    for(int j = 0;j<g->taille;j++) {

        int val1 = g->sommet[j].op1.indice;
        int val2 = g->sommet[j].op2.indice;

        ////CAS DU 0
        if(g->sommet[j].op1.val == 0 || g->sommet[j].op2.val == 0){
            g->adj[val1][val2] = 0;
            g->adj[val2][val1] = 0;

        }

        g->adj[val1][val2] = 1;
        g->adj[val2][val1] = 1;


}


    //welsh et powell



    int* color = (int*) calloc(g->ordre,sizeof(int)); //0 partout si pas de couleur
    int couleur = 1;

// dans la matrice ADJ; indice dans liste = indice dans ADJ

    for(int i = 0; i<g->ordre; i++) {

        if (color[i] == 0) { //si non colorer
            color[i] = couleur;


            for (int j = i+1; j < g->ordre; j++) { // coloration de sommet en fonction de lien avec s1 ou non


                if ((g->adj[i][j] == 0 || deg[j] == 0) && color[j] == 0 ) { // pas de lien avec s1

                    color[j] = couleur;


                    for (int l = 0; l < j; l++) {
                        int s3 = g->liste[l];
                        if (g->adj[j][l] == 1  && color[l] == color[j] ) { // lien entre sommet de meme couleur

                            if(g->liste[j] == 0 || s3 == 0){
                                break;
                            }

                            color[l] = 0;  //changment de couleur

                            break;
                        }



                    }
                }

            }
            couleur++;
        }
    }



/////////////////////////////

    printf("\nIl y aura besoin de %d stations",couleur-1);


    for(int i =1; i< couleur;i++){
        printf("\nSTATION %d: ",i);
        for (int j=0; j<g->ordre;j++){
            if (color[j] == i){
                if(g->liste[j] == 0){ //sommet inutile
                   printf("");
                }
                else {
                    printf("%d - ", g->liste[j]);
                }
            }
        }
    }

    free(color);

    return g;
}




int main() {
    graphe * g=(graphe*)malloc(sizeof(graphe));
    g->sommet = NULL;
    g->liste = NULL;
    g->adj = NULL;
    char fichier[100];
    printf("ENTREZ LE NOM DU FICHIER (avec le .txt):");
    scanf("%s",fichier);
    FILE * ifs = fopen(fichier,"r");

    if (!ifs)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }

    g->ordre = 31; //ordre + 1 pour le sommet 0

    Graphe(ifs,g);


    free(g->sommet);
    free(g->liste);
    for(int i=0; i<g->ordre; i++){
        free(g->adj[i]);
    }
    free(g);
    fclose(ifs);

    return 0;
}