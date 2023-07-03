#pragma once

#include "Constantes.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PROCHE 1
#define MOYEN 2
#define LOIN 3
#define AUCUN 4

#define EGAUCHE 1
#define GAUCHE 2
#define CENTRE 3
#define DROITE 4
#define EDROITE 5

typedef int regle[6];
typedef regle bot[10];

#define DIRGAUCHE -1
#define DIRMILIEU 0
#define DIRDROITE 1
