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
void drawText(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color color, int x, int y);
SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filepath);
void displayHelp();
void displayHighScore();

#endif //PROJET_ZZ1_MENU_H
