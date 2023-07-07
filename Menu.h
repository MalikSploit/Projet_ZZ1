#ifndef PROJET_ZZ1_MENU_H
#define PROJET_ZZ1_MENU_H

#include "Constantes.h"
#include "jeu.h"
#include "simulation.h"
#include "jeu_SDL.h"


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
void displayScoreboard(SDL_Renderer* renderer);
void calculerMoyenneScores();
void cleanup0(SDL_Renderer* renderer, SDL_Window* window, Button buttons[], TTF_Font* font, Mix_Music *bgMusic, Mix_Chunk* buttonSound, SDL_Texture* logo1, SDL_Texture* logo2, SDL_Texture* bgTexture1, SDL_Texture* bgTexture2);

#endif //PROJET_ZZ1_MENU_H
