#pragma once

#include "Constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/* #include <strings.h> */

#include <math.h>

// types regle et bot

#define TAILLE_ETAT 4 // nombre de perceptions pour les regles du bot
#define NB_REGLES 10 // nombre de regles dans un bot
#define IMPORTANCE_PRIORITES 5

/* Une regle contient six entiers :
   - la proximite de l'obstacle le plus bas sur la colonne à gauche de la proie
   - la proximite de l'obstacle le plus bas sur la colonne de la proie
   - la proximite de l'obstacle le plus bas sur la colonne à droite de la proie
   - la position de la proie par rapport au chasseur
   - l'action du bot (dirgauche, dirmilieu, dirdroite)
   - la priorite entre 1 et 5 (5 : plus important)
*/
typedef int regle[TAILLE_ETAT + 2];
typedef regle bot[NB_REGLES];

// Valeurs pour les proximites des plus proches obstacles
#define PROCHE 1 // immediatement sur la ligne suivante
#define MOYEN 2 // entre 2 et 4 lignes plus loin
#define LOIN 3 // plus de 4 lignes
#define AUCUN 4 // pas d'obstacle

// Valeurs pour la position de la proie
#define EGAUCHE 1 // à gauche mais pas directement
#define GAUCHE 2 // immédiatement à gauche
#define CENTRE 3 // devant
#define DROITE 4 // immédiatement à droite
#define EDROITE 5 // à droite mais pas directement

// Directions possibles de deplacement (outputs d'une regle)
#define DIRGAUCHE -1
#define DIRMILIEU 0
#define DIRDROITE 1

/* Comportement de la proie */

/* Probabilité de fuir le prédateur */
#define INFLUENCEPREDATEUR 0.4
/* Probabilité d'avoir un comportement aléatoire */
#define ALEATOIRE 0.1

// prototypes
bool iterJeu(jeu j, int deplacement);
void getSituationFromJeu(jeu j, int situation[TAILLE_ETAT]);
int deplacementFromBot(jeu j, bot robot, int situation[TAILLE_ETAT]);
int distanceSurColonne(jeu j, int colonne);
int proximiteSurColonne(jeu j, int colonne);
void avanceGrille(int grille[][NB_COLONNES]);
void creerLigne(int arr[NB_COLONNES]);
int obstacleDroite(jeu j);
int obstacleGauche(jeu j);
bool jeuFini(jeu j);
bool verifCroix(int debut, int fin, int * ligne);
bool chasseurBienPlace(int chasseur, int debut, int fin);
bool verifDeplacement(int grille[][NB_COLONNES], int deplacement, int coordonnee, int ligne);
void avanceGrille(int grille[][NB_COLONNES]);
void creerLigne(int arr[NB_COLONNES]) ;
int retourneDeplacement(jeu j, int numLigne, int deplacement);
void deplacer(jeu j, int deplacement, int numLigne);
void deplacerChasseur(jeu j, int deplacement);
void deplacerProie(jeu j);
int comportementProie(jeu j);
