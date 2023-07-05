#ifndef PROJET_ZZ1_MENU_H
#define PROJET_ZZ1_MENU_H

#include "Constantes.h"

typedef struct Button
{
    SDL_Rect rect;
    SDL_Color color;
    char* text;
    TTF_Font* font;
    SDL_Texture* texture;
} Button;


typedef struct
{
    char name[50];
    int score;
} PlayerScore;


void drawButton(SDL_Renderer* renderer, Button* button);
SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filepath);
void displayHelp(SDL_Renderer* renderer);
void displayHighScore(SDL_Renderer* renderer);

#endif //PROJET_ZZ1_MENU_H
