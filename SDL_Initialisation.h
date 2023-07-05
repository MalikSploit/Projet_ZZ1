#ifndef PROJET_ZZ1_SDL_INITIALISATION_H
#define PROJET_ZZ1_SDL_INITIALISATION_H

#include "Constantes.h"

SDL_Window* createWindow(char *title);
SDL_Renderer* createRenderer(SDL_Window* window);
SDL_Texture* loadLogo(SDL_Renderer* renderer, char *imagePath);
TTF_Font* loadFont(char* fontPath, int size);
int initializeSDL();
int initializeTTF();
int initializeIMG();
SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filepath);
SDL_Surface* LoadImage(char *imagePath);
SDL_Texture *LoadTexture(SDL_Renderer *renderer, SDL_Surface *backgroundSurface);
SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* text);
#endif //PROJET_ZZ1_SDL_INITIALISATION_H
