#ifndef PROJET_ZZ1_CONSTANTES_H
#define PROJET_ZZ1_CONSTANTES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NB_LIGNES 8
#define NB_COLONNES 8
#define TAILLE_CELLULE_LARGEUR 361
#define TAILLE_CELLULE_LONGUEUR 243
#define MAX_OBSTACLES 6
#define MAX_ITER 1000
#define NOMBRE_SPRITE 8
#define PROBA_OBSTACLE 0.15
#define MAX_LINE_LENGTH 150
#define MAX_NAME_LENGTH 50

typedef struct {
    int grille[NB_LIGNES][NB_COLONNES]; // 0 : case vide, 1 : obstacle
    int chasseur;
    int proie;
} jeu;

#endif //PROJET_ZZ1_CONSTANTES_H
