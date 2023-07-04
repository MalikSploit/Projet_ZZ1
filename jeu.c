#include "jeu.h"

/* renvoie le score. Simule le jeu et run une de ces instance */
int Jeu(bot robot){
    bool fin = false;
    int i;
    int deplacement;
    int situation[TAILLE_ETAT];
    
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
	deplacement = deplacementFromBot(j, robot, situation);
	fin = iterJeu(j, deplacement);
	i++;
    }
    return i;
}

// renvoie la distance entre la proie et le plus proche obstacle sur la colonne donnee
// s'il n'y pas d'obstacle, renvoie NB_LIGNES-1
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
    // si pas d'obstacle, on renvoie l'indice de la ligne après la dernière
    if(!trouve) ligne = NB_LIGNES;
   
    // renvoyer la distance à la proie
    return ligne-1;
}

// renvoie la proximite de l'obstacle sur la colonne donnee
int proximiteSurColonne(jeu j, int colonne) {
    // recuperer la distance sur la colonne
    int distance = distanceSurColonne(j, colonne);

    // mettre a jour la situation en fonction du cas
    int result;
    if (distance == 1) result = PROCHE;
    else if (distance <= 4) result = MOYEN;
    else if (distance < NB_LIGNES - 1) result = LOIN;
    else result = AUCUN;

    return result;
}

// modifier la situation en fonction du jeu
void getSituationFromJeu(jeu j, int situation[TAILLE_ETAT]){
    // colonne a gauche de la proie
    if (j.proie == 0) {
	// si la proie est collee au bord, c'est comme s'il y avait un obstacle sur la case suivante
	situation[0] = PROCHE;
    } else {
	situation[0] = proximiteSurColonne(j, j.proie-1);
    }

    // colonne d'en face
    situation[1] = proximiteSurColonne(j, j.proie);

    // colonne a droite de la proie
    if (j.proie == NB_COLONNES - 1) {
	// si la proie est collee au bord, c'est comme s'il y avait un obstacle sur la case suivante
	situation[2] = PROCHE;
    } else {
	situation[2] = proximiteSurColonne(j, j.proie+1);
    }

    // determination de la position de la proie
    if (j.proie == j.chasseur) situation[3] = CENTRE;
    else if (j.proie == j.chasseur - 1) situation[3] = GAUCHE;
    else if (j.proie == j.chasseur + 1) situation[3] = DROITE;
    else if (j.proie < j.chasseur) situation[3] = EGAUCHE;
    else situation[3] = EDROITE;
}

// teste si une regle matche une situation (en prenant en compte les jokers -1 dans la regle)
bool matchRegleSituation(regle r, int situation[TAILLE_ETAT]) {
    bool matches = true;
    int i = 0;
    while (matches && i < TAILLE_ETAT) {
	if(situation[i] != r[i] && r[i] != -1) matches = false;
	i++;
    }
    return matches;
}

// renvoyer ce que fait le bot dans le cas donne
int deplacementFromBot(jeu j, bot robot, int situation[TAILLE_ETAT]){
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
        arr[j] = rand() % 2;
    }
}
