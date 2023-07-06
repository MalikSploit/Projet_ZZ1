#ifndef PROJET_ZZ1_MENU_H
#define PROJET_ZZ1_MENU_H

#include "Constantes.h"
#include "jeu.h"
#include "simulation.h"

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
    char playerName[MAX_NAME_LENGTH];
    int playerScore;
    char botName[MAX_NAME_LENGTH];
    int botScore;
} HighScores;


void drawButton(SDL_Renderer* renderer, Button* button);
void displayHelp(SDL_Renderer* renderer);
void displayHighScore(SDL_Renderer* renderer);
void initHighScore();

#endif //PROJET_ZZ1_MENU_H
