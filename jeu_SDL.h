#ifndef PROJET_ZZ1_MAIN_H
#define PROJET_ZZ1_MAIN_H

#include "glouton.h"
#include "Menu.h"
#include "SDL_Initialisation.h"
#include "Constantes.h"

typedef struct UserCar
{
    SDL_Rect rect;
    SDL_Texture *textures[3];
    int cell_x;
    int cell_y;
    int velocity;
    int current_frame;
    int total_frames;
    double draw_x;
    double draw_y;
} UserCar;

typedef struct
{
    int cell_x;
    int cell_y;
    SDL_Rect rect;
    SDL_Texture *texture;
    int speed;
} EnemyCar;


extern SDL_Texture* obstacles[NOMBRE_SPRITE];

void LancerJeu(SDL_Renderer* renderer, Mix_Chunk* buttonSound);
UserCar initVoiture(SDL_Renderer *renderer, int x, int y);
void drawVoiture(SDL_Renderer *renderer, UserCar *userCar);
void updateText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor, SDL_Texture** texture, SDL_Rect* rect, const char* text);
void cleanup(SDL_Surface* backgroundSurface, SDL_Texture* backgroundTexture, SDL_Texture* backgroundTexture2,
             SDL_Texture* scoreTexture, SDL_Texture* pauseTexture, SDL_Texture* vitesseTexture, SDL_Texture* highScoreTexture, TTF_Font* font, TTF_Font* gameOverFont,
             UserCar userCar, UserCar moto, SDL_Texture* obstacles[]);
void InitScore(SDL_Renderer* renderer, int* score, TTF_Font* font, SDL_Texture** scoreTexture, SDL_Rect *scoreRect, SDL_Color* textColor, int deplacementEffectue);
void initVitesse(UserCar* userCar, SDL_Renderer *renderer, SDL_Color *textColor, SDL_Texture** vitesseTexture, SDL_Rect *vitesseRect, TTF_Font* font);
int gameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score);


#endif //PROJET_ZZ1_MAIN_H
