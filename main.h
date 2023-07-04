#ifndef PROJET_ZZ1_MAIN_H
#define PROJET_ZZ1_MAIN_H

#include "Constantes.h"

#define TAILLE_CELLULE_LARGEUR 361
#define TAILLE_CELLULE_LONGUEUR 243
#define MAX_OBSTACLES 1

typedef struct UserCar
{
    SDL_Rect rect;
    SDL_Texture *textures[3];
    int cell_x;
    int cell_y;
    int velocity;
    int current_frame;
    int total_frames;
} UserCar;

typedef struct
{
    int cell_x;
    int cell_y;
    SDL_Rect rect;
    SDL_Texture *texture;
    int speed;
    Uint32 last_update; // New field for time of last update;
} EnemyCar;


int LancerJeu();
UserCar initVoiture(SDL_Renderer *renderer);
EnemyCar initObstacle(SDL_Renderer *renderer, int lane_x, int lane_y, char *obstacleImagePath);
bool checkCollision(int grid[8][8], int x, int y);
EnemyCar initRandomObstacle(SDL_Renderer *renderer, int grid[8][8]);
void drawObstacle(SDL_Renderer *renderer, EnemyCar *enemyCar);
void drawVoiture(SDL_Renderer *renderer, UserCar *userCar);
void updateText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor, SDL_Texture** texture, SDL_Rect* rect, const char* text);
void moveObstacles(SDL_Renderer *renderer, int grid[8][8], EnemyCar *obstacles, int *numObstacles, UserCar *userCar, bool *running);
void cleanup(SDL_Surface* backgroundSurface, SDL_Texture* backgroundTexture, SDL_Texture* backgroundTexture2,
             SDL_Texture* scoreTexture, SDL_Texture* pauseTexture, SDL_Texture* vitesseTexture, TTF_Font* font,
             UserCar userCar, EnemyCar obstacles[], SDL_Renderer* renderer, SDL_Window* window);
void InitScore(SDL_Renderer* renderer, Uint32* lastScoreUpdateTime, int* score, TTF_Font* font, SDL_Texture** scoreTexture, SDL_Rect *scoreRect, UserCar* userCar, SDL_Color* textColor);
void initVitesse(UserCar* userCar, SDL_Renderer *renderer, SDL_Color *textColor, SDL_Texture** vitesseTexture, SDL_Rect *vitesseRect, TTF_Font* font);


#endif //PROJET_ZZ1_MAIN_H
