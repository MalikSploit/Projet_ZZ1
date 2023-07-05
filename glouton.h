#pragma once

#include "Constantes.h"
#include "jeu.h"
#include <stdio.h>
#include "pthread.h"

#define NOMBRE_GLOUTON 100
#define NUM_THREADS 1
#define NOMBRE_INDICES NB_REGLES*(TAILLE_ETAT+2)

void initialiserBot(bot unBot);
void algoGlouton(bot unBot);
void melangeIndices(int * indices);
void initIndices(int * indices);
void swap(int *a, int *b);
