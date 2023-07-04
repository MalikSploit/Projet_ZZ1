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
	/* déplacement toujours faisable */
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

    float probabilities[3];
    float probaTotale = 0;
    float newProba;

    bool regleExiste = false;

    for (int i = 0; i < NB_REGLES; i++) {
	// si la regle matche et que le deplacement est legal on attribue la probabilite
	if(matchRegleSituation(robot[i], situation) && verifDeplacement(j.grille, robot[i][TAILLE_ETAT], j.chasseur, 0)) {
	    newProba = powf(robot[i][TAILLE_ETAT+1], IMPORTANCE_PRIORITES);
	    probabilities[robot[i][TAILLE_ETAT]] += newProba;
	    probaTotale += newProba;
	    regleExiste = true;
	}
    }
    
    // si aucune regle n'a matche on attribue des probas egales a tous les deplacements legaux
    if (!regleExiste) {
	for (int i = -1; i < 2; i++) {
	    if(verifDeplacement(j.grille, i, j.chasseur, 0)) {
		probabilities[i] = 1;
		probaTotale += 1;
	    }
	}
    }

    // choix d'une direction en fonction des probabilites

    double r = ((double)rand() / RAND_MAX) * probaTotale;
    int direction = -2;

    for(int dir = -1; dir < 2 && r > 0; dir++) {
        if(probabilities[dir+1] > 0) {
            r -= probabilities[dir+1];
            if(r <= 0) {
                direction = dir;
            }
        }
    }

    // gestion de l'imprecision des flottants : si on n'a pas trouve, c'est le dernier possible
    if (direction == -2) {
	int dir = 1;
	while (dir > -2 && direction == -2) {
	    // si c'est un cas possible on le prend
	    if(probabilities[dir+1]) direction = dir;
	    dir--; // parcours en sens inverse
	}
    }

    return direction;
}

bool iterJeu(jeu j, int deplacement){

  deplacerProie(j);
  deplacerChasseur(j, deplacement);
  avanceGrille(j.grille);
  return jeuFini(j);
  
}

void deplacerChasseur(jeu j, int deplacement){
  deplacer(j, deplacement, 0);
}

void deplacerProie(jeu j){

  /* calcul du comportement de la proie */
  int deplacement = comportementProie(j);
  /* vérifier tant que */
  deplacer(j, deplacement, 1);

}

/* retourne un déplacement, donc un entier */
int comportementProie(jeu j){

  int deplacement;

  float p = (float)rand()/(float)RAND_MAX;  // Génère un nombre aléatoire entre 0 et 1

  if (p < INFLUENCEPREDATEUR) {
    /* si le chasseur est derrière la proie */
    if(j.chasseur == j.proie){
      deplacement = (rand() % 2) * 2 - 1;  // Génère soit -1 soit 1
    }
    else if(j.chasseur < j.proie){
      if(verifDeplacement(j.grille, DIRDROITE, j.proie, 1)){
	deplacement = DIRDROITE;
      }
      else if(verifDeplacement(j.grille, DIRMILIEU, j.proie, 1)){
	deplacement = DIRMILIEU;
      }
      else deplacement = DIRGAUCHE;
    }
  } 
  else if (p < INFLUENCEPREDATEUR + ALEATOIRE) {
    deplacement = (rand() % 3) - 1;  // Génère un nombre aléatoire entre -1 et 1;
  } 
  else {
    /* déplacement "intelligent" = prend la direction ou la croix est la plus loin*/

    int valDIRGAUCHE = distanceSurColonne(j, j.proie - 1);
    int valDIRMILIEU = distanceSurColonne(j, j.proie);
    int valDIRDROITE = distanceSurColonne(j, j.proie + 1);

    /* choix du + grand */
    deplacement = DIRGAUCHE;
    int max = valDIRGAUCHE;
    if (valDIRDROITE > max) {
      deplacement = DIRDROITE;
      max = valDIRDROITE;
    }
    if (valDIRMILIEU > max) {
      deplacement = DIRMILIEU;
    }
  }
  return deplacement;
}

int retourneDeplacement(jeu j, int numLigne, int deplacement){

  if(deplacement == DIRGAUCHE){
    while(j.grille[numLigne + 1][j.chasseur + deplacement] == 1)
      deplacement--;
  }
  else if (deplacement == DIRDROITE){
    while(j.grille[numLigne + 1][j.chasseur + deplacement] == 1)
      deplacement++;
}

  return deplacement;
  
}

void deplacer(jeu j, int deplacement, int numLigne){

  /* on bouge le prédateur */
  if(numLigne == 0){

    deplacement = retourneDeplacement(j, 0, deplacement);

    j.chasseur = j.chasseur + deplacement;

  }

  /* on bouge la proix */
  else {

    deplacement = retourneDeplacement(j, 1, deplacement);
    j.proie = j.proie + deplacement;

  }
}

int obstacleGauche(jeu j) {
    int i = j.proie - 1;
    while (i >= 0 && !j.grille[1][i]) {
	i--;
    }
    return i;
}

int obstacleDroite(jeu j) {
    int i = j.proie + 1;
    while (i < NB_COLONNES && !j.grille[1][i]) {
	i++;
    }
    return i;
}

bool jeuFini(jeu j){

  int posObstacleGauche = obstacleGauche(j);
  int posObstacleDroite = obstacleDroite(j);

  if(verifCroix(posObstacleGauche, posObstacleDroite, j.grille[2]) && chasseurBienPlace(j.chasseur, posObstacleGauche, posObstacleDroite)){
    return true;
  }
  else return false;
}

/* retourne 1 si les croix sont bien toutes présentes */
bool verifCroix(int debut, int fin, int * ligne){
  debut++;
  while(debut < fin) {
    if(ligne[debut] == 0)return 0;
      debut++;
  }
  return 1;
}

/* retourne 1 si les croix sont bien toutes présentes */
bool chasseurBienPlace(int chasseur, int debut, int fin){
 if (chasseur > debut && chasseur < fin) {
   return 1;
    }
 else return 0;
}

/* renvoie 1 si VALIDE, renvoie 0 si PAS VALIDE, renvoie -1 si téléportation */
bool verifDeplacement(int grille[][NB_COLONNES], int deplacement, int coordonnee, int ligne){

  /* verif côté gauche */
  if(coordonnee + deplacement < 0) return 0;
  /* verif côté droit */
  if(coordonnee + deplacement > NB_COLONNES - 1) return 0;
  /* verif en à la ligne N + 1 que la case est vide où il veut aller  */
  if(grille[ligne + 1][coordonnee + deplacement] == 1) return 0;

  /* si les trois cases devant lui sont prises, alors cas spécial de téléportation */
  if((grille[ligne + 1][coordonnee + DIRGAUCHE] == 1)
     &&
     (grille[ligne + 1][coordonnee + DIRMILIEU] == 1)
     &&
     (grille[ligne + 1][coordonnee + DIRDROITE] == 1))
    {
    int indiceOuAller = 0;
    if (deplacement == DIRMILIEU)
      return 0;
    else if (deplacement == DIRGAUCHE) {
      while (grille[ligne + 1][indiceOuAller] == 1) {
        indiceOuAller--;
      }
      if (indiceOuAller < 0 && indiceOuAller > NB_COLONNES - 1)
        return 0;
      else
        return -1;
      }
      else if(deplacement == DIRDROITE){
        while (grille[ligne + 1][indiceOuAller] == 1) {
          indiceOuAller++;
        }
        if (indiceOuAller < 0 && indiceOuAller > NB_COLONNES - 1)
          return 0;
        else return -1;
      }
    }

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
    creerLigne(grille[NB_LIGNES - 1]);


}

void creerLigne(int arr[NB_COLONNES]) {
    srand(time(0)); // Initialise le générateur de nombres aléatoires

    // Remplis le tableau avec des nombres aléatoires 0 et 1
    for (int j = 0; j < NB_COLONNES; j++) {
      arr[j] = rand() % NOMBRE_SPRITE + 1;
    }
}
