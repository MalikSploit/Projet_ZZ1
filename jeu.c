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
        arr[j] = rand() % 2;
    }
}
