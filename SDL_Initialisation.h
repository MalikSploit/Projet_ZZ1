#ifndef PROJET_ZZ1_SDL_INITIALISATION_H
#define PROJET_ZZ1_SDL_INITIALISATION_H


#include "Constantes.h"

SDL_Window* createWindow(char *title);
SDL_Renderer* createRenderer(SDL_Window* window);
int initializeSDL();
int initializeTTF();
int initializeIMG();
#endif //PROJET_ZZ1_SDL_INITIALISATION_H
