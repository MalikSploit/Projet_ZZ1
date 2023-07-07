#ifndef PROJET_ZZ1_MAIN_H
#define PROJET_ZZ1_MAIN_H

#include "glouton.h"
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
    Uint32 last_frame_time;
} UserCar;


extern SDL_Texture* obstacles[NOMBRE_SPRITE];

void LancerJeu(SDL_Renderer* renderer, bot robot, char * botname);
UserCar initVoiture(SDL_Renderer *renderer, int x, int y);
void drawVoiture(SDL_Renderer *renderer, UserCar *userCar);
void updateText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor, SDL_Texture** texture, SDL_Rect* rect, const char* text);
void cleanup(SDL_Surface* backgroundSurface, SDL_Texture* backgroundTexture, SDL_Texture* backgroundTexture2,
             SDL_Texture* scoreTexture, SDL_Texture* pauseTexture, SDL_Texture* vitesseTexture, SDL_Texture* highScoreTexture, TTF_Font* font, TTF_Font* gameOverFont,
             UserCar userCar, UserCar moto, SDL_Texture* obstacle[], Mix_Chunk* buttonSound, char *username);
void InitScore(SDL_Renderer* renderer, int* score, TTF_Font* font, SDL_Texture** scoreTexture, SDL_Rect *scoreRect, SDL_Color* textColor, int deplacementEffectue);
void initVitesse(UserCar* userCar, SDL_Renderer *renderer, SDL_Color *textColor, SDL_Texture** vitesseTexture, SDL_Rect *vitesseRect, TTF_Font* font);
char* DemanderQqch(SDL_Renderer* renderer, int *QuitterJeu, char *pathBackgroud, char *pathMessage);
void playButtonSound(Mix_Chunk* buttonSound);
void initHighScore();
UserCar initMoto(SDL_Renderer *renderer, int x, int y);
void drawMoto(SDL_Renderer *renderer, UserCar *moto);
void initTexturesObstacles(SDL_Renderer *renderer);
void drawObstacles(SDL_Renderer *renderer, jeu g);
int getHighScore();
void logScore(const char* username, int score, bool humain);
int gameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score, int highScore);

#endif //PROJET_ZZ1_MAIN_H
