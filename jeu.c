#include "jeu.h"

/* renvoie le score. Simule le jeu et run une de ces instance */
int Jeu(bot robot){
    bool fin = false;
    int i;
    int deplacement;
    int situation[4];
    
    // initialisation du jeu
    jeu j;
    for (int y = 0; y < NB_LIGNES; y++) {
	for (int x = 0; x < NB_COLONNES; x++) {
	    j.grille[y][x] = 0;
	}
    }
    j.chasseur = rand() % NB_COLONNES;
    j.proie = rand() % NB_COLONNES;

    while (i > MAX_ITER && !fin) {
	getSituationFromJeu(j, situation);
	deplacement = deplacementFromBot(robot, situation);
	fin = iterJeu(j, deplacement);
	i++;
    }
    return i;
}

void getSituationFromJeu(jeu j, int situation[4]){
    // modifier la situation en fonction du jeu
}

int deplacementFromBot(bot robot, int situation[4]){
    // renvoyer ce que fait le bot dans le cas donne
    return DIRMILIEU;
}

bool iterJeu(jeu j, int deplacement){

  /* SDL */
  /* iterJeu(grille, proie, chasseur, deplacement) */

  /* for (i=0; i < MAX_ITER; i++) { */
/* iterJeu(grille, proie, chasseur, deplacement) */
/* } */

    // appel à decalerGrille qui genere une nouvelle ligne et decale les autres
}
