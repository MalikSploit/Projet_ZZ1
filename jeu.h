#pragma once

#include "Constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


// Valeurs pour les distances des plus proches obstacles
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

// types regle et bot
/* Une regle contient six entiers :
   - la distance de l'obstacle le plus bas sur la colonne à gauche de la proie
   - la distance de l'obstacle le plus bas sur la colonne de la proie
   - la distance de l'obstacle le plus bas sur la colonne à droite de la proie
   - la position de la proie par rapport au chasseur
   - l'action du bot (dirgauche, dirmilieu, dirdroite)
   - la priorite entre 1 et 5 (5 : plus important)
*/
typedef int regle[6];
typedef regle bot[10];

// Directions possibles de deplacement (outputs d'une regle)
#define DIRGAUCHE -1
#define DIRMILIEU 0
#define DIRDROITE 1

// prototypes
bool iterJeu(jeu j, int deplacement);
void getSituationFromJeu(jeu j, int situation[4]);
int deplacementFromBot(bot robot, int situation[4]);
int distanceSurColonne(jeu j, int colonne);
void avanceGrille(int grille[][NB_COLONNES]);
void creerLigne(int arr[NB_COLONNES]);
