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
    int ligne = 1;
    bool trouve = false;
    // recherche du plus proche obstacle
    while (ligne < NB_LIGNES && !trouve) {
	ligne++;
	if(j.grille[ligne][colonne]) {
	    trouve = true;
	}
    }
    // mettre a jour la situation en fonction du cas
    int result;
    if (trouve){
	if (ligne == 2) result = PROCHE;
	else if (ligne <= 5) result = MOYEN;
	else result = LOIN;
    } else result = AUCUN;

    return result;
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

  deplacerProie();
  déplacerChasseur();
  avanceGrille(j.grille);
  return jeuFini(j);
  
}

bool jeuFini(jeu j){


  int posProie = j.proie;

  int posObstacleGauche = obstacleGauche(posProie);
  int posObstacleDroite = obstacleDroite(posProie);

  if((posObstacleGauche && posObstacleDroite) && verifierSurLesCOlonneETLigne)
}

/* renvoie 1 si VALIDE, renvoie 0 si PAS VALIDE */
bool verifDeplacement(int grille[][NB_COLONNES], int deplacement, int coordonnee, int ligne){

  /* verif côté gauche */
  if(coordonnee + deplacement < 0) return 0;
  /* verif côté droit */
  if(coordonnee + deplacement > NB_COLONNES - 1) return 0;
  /* verif en à la ligne N + 1 qui faut vide  */
  if(grille[ligne + 1] coordonnee + deplacement == ) return 0;
  return 1;

}

void avanceGrille(int grille[][NB_COLONNES]){
  
    srand(time(0)); // Initialise le générateur de nombres aléatoires

    // Déplace toutes les lignes une ligne vers le bas
    for (int i = 0; i < NB_LIGNES - 1; i++) {
        for (int j = 0; j < NB_COLONNES; j++) {
            grille[i][j] = grille[i + 1][j];
        }
    }

    // Remplis la dernière ligne avec des nombres aléatoires 0 et 1
    for (int j = 0; j < NB_COLONNES; j++) {
        grille[NB_LIGNES - 1][j] = rand() % 2;
    }

    int derniere[NB_COLONNES] = {0};
    creerLigne(derniere);

    for (int j = 0; j < NB_COLONNES; j++) {
      grille[NB_LIGNES - 1][j] = derniere[j];
    }

}

void creerLigne(int arr[NB_COLONNES]) {
    srand(time(0)); // Initialise le générateur de nombres aléatoires

    // Remplis le tableau avec des nombres aléatoires 0 et 1
    for (int j = 0; j < NB_COLONNES; j++) {
      arr[j] = rand() % NOMBRE_SPRITE + 1;
    }
}
