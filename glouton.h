#pragma once

#include "Constantes.h"
#include <stdio.h>
#include <jeu.h>

#define NOMBRE_GLOUTON 10
#define NOMBRE_INDICES 40 // NB_REGLES * TAILLE_ETAT

bot creerBot();
bot algoGlouton(bot unBot);
void * permutation(int * indices);
void * initIndices(int * indices);
void swap(int *a, int *b);
