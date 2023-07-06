#pragma once

#include "glouton.h"
#include "jeu_SDL.h"

void lancerSimulation();
void recupererBot(SDL_Renderer* renderer, int * quitterJeu, bot leBot);
void recupererNomBot(SDL_Renderer* renderer, int * quitterJeu, char *nomBot);
