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

int distanceSurColonne(jeu j, int colonne) {
    return PROCHE;
}

// modifier la situation en fonction du jeu
void getSituationFromJeu(jeu j, int situation[4]){
    // colonne a gauche de la proie
    if (j.proie == 0) {
	// si la proie est collee au bord, c'est comme s'il y avait un obstacle sur la case suivante
	situation[0] = PROCHE;
    } else {
	situation[0] = distanceSurColonne(j, j.proie-1);
    }

    // colonne d'en face
    situation[1] = distanceSurColonne(j, j.proie);

    // colonne a droite de la proie
    if (j.proie == NB_COLONNES - 1) {
	// si la proie est collee au bord, c'est comme s'il y avait un obstacle sur la case suivante
	situation[2] = PROCHE;
    } else {
	situation[2] = distanceSurColonne(j, j.proie+1);
    }

    // determination de la position de la proie
    if (j.proie == j.chasseur) situation[3] = CENTRE;
    else if (j.proie == j.chasseur - 1) situation[3] = GAUCHE;
    else if (j.proie == j.chasseur + 1) situation[3] = DROITE;
    else if (j.proie < j.chasseur) situation[3] = EGAUCHE;
    else situation[3] = EDROITE;
}

// renvoyer ce que fait le bot dans le cas donne
int deplacementFromBot(bot robot, int situation[4]){
    // trouver les regles qui matchent la situation
    // retirer les regles demandant une action illegale
    // choisir une regle parmi les restantes en priorisant les priorités fortes (proba de prio^s / somme des prio^s avec s constante d'importance des priorites)
    // si aucune regle candidate, choisir un deplacement random parmi les legaux
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
