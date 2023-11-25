#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}


graphe* lecture2(char* fichier){
    {

        graphe * g=(graphe*)malloc(sizeof(graphe));
        FILE * ifs = fopen(fichier,"r");
        int taille, ordre, s1, s2,poids;

        if (!ifs)
        {
            printf("Erreur de lecture fichier\n");
            exit(-1);
        }

        fscanf(ifs,"%d",&ordre);

        printf("Affichage du fichier\n");



        printf("ordre: %d\n", ordre);


        fscanf(ifs,"%d",&taille);
        printf("taille: %d\n",taille);

        g->ordre=ordre;
        g->taille=taille;

        g->arbre = (arette*)malloc(taille*sizeof(arette));

        // créer les arêtes du graphe
        for (int i=0; i<taille; ++i)
        {
            fscanf(ifs,"%d%d%d",&s1,&s2,&poids);

            printf("Distance entre %d et %d: %d\n",s1,s2,poids);
            g->arbre[i].s1 = s1;
            g->arbre[i].s2 = s2;
            g->arbre[i].poids = poids;

        }

        return g;
    }
}
