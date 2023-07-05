#include "jeu_SDL.h"
#include "SDL_Initialisation.h"
#include "Constantes.h"
#include "jeu.h"

SDL_Texture* obstacles[NOMBRE_SPRITE];

//Fonction pour initialiser le chasseur
UserCar initVoiture(SDL_Renderer *renderer, int x, int y)
{
    UserCar userCar;

    // Set up initial size and position of the user car
    userCar.cell_x = x;
    userCar.cell_y = 7 - y;
    userCar.rect.w = 140;
    userCar.rect.h = 130;
    userCar.current_frame = 0;
    userCar.total_frames = 3;

    // Load the textures for the user car
    char* image_files[] = {"Images/Chasseur/1.png", "Images/Chasseur/2.png", "Images/Chasseur/3.png"};
    for (int i = 0; i < userCar.total_frames; ++i)
    {
        SDL_Surface *tempSurface = LoadImage(image_files[i]);

        userCar.textures[i] = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }

    // Initial velocity
    userCar.velocity = 2;

    return userCar;
}

// Fonction pour initialiser la proie
UserCar initMoto(SDL_Renderer *renderer, int x, int y)
{
    UserCar moto;

    // Configurez la taille et la position initiale de la moto
    moto.cell_x = x;
    moto.cell_y = 7 - y;
    moto.rect.w = 70;
    moto.rect.h = 100;

    // Chargez la texture pour la moto
    SDL_Surface *tempSurface = LoadImage("Images/Proie.png");

    moto.textures[0] = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return moto;
}

// Fonction pour dessiner la moto à l'écran
void drawMoto(SDL_Renderer *renderer, UserCar *moto)
{
    // Mettez à jour la position du rectangle en fonction de la position cellulaire
    moto->rect.x = moto->cell_x * (TAILLE_CELLULE_LARGEUR/2) + 285;
    moto->rect.y = moto->cell_y * (TAILLE_CELLULE_LONGUEUR/2); // start at the bottom of the screen

    // Dessinez la texture de la moto à l'écran
    SDL_RenderCopy(renderer, moto->textures[0], NULL, &(moto->rect));
}


void initTexturesObstacles(SDL_Renderer *renderer) {
    for(int i = 0; i < NOMBRE_SPRITE; i++) {
	char obstacleImagePath[25];
	sprintf(obstacleImagePath, "Images/Obstacle%d.png", i+1);
	obstacles[i] = loadTexture(renderer, obstacleImagePath);
    }
}

void drawObstacles(SDL_Renderer *renderer, jeu g){
    for(int i = 0; i < NB_LIGNES; i++){
	for(int j = 0; j < NB_COLONNES; j++) {
	    if(g.grille[i][j]) {
		SDL_Rect posObstacle = {j * (TAILLE_CELLULE_LARGEUR/2) + 250,
		    (7-i) * (TAILLE_CELLULE_LONGUEUR/2),
		    140,
		    130
		};
		SDL_RenderCopy(renderer, obstacles[g.grille[i][j]-1], NULL, &posObstacle);
	    }
	}
    }
}

void drawObstacle(SDL_Renderer *renderer, EnemyCar *enemyCar)
{
    enemyCar->rect.x = enemyCar->cell_x * (TAILLE_CELLULE_LARGEUR/2) + 250;
    enemyCar->rect.y = enemyCar->cell_y * (TAILLE_CELLULE_LONGUEUR/2);

    // Render the enemy car
    SDL_RenderCopy(renderer, enemyCar->texture, NULL, &enemyCar->rect);
}

void drawVoiture(SDL_Renderer *renderer, UserCar *userCar)
{
    userCar->rect.x = userCar->cell_x * (TAILLE_CELLULE_LARGEUR/2) + 250;
    userCar->rect.y = userCar->cell_y * (TAILLE_CELLULE_LONGUEUR/2); // start at the bottom of the screen
    SDL_RenderCopy(renderer, userCar->textures[userCar->current_frame], NULL, &userCar->rect);
    // Update the frame for the next draw
    userCar->current_frame = (userCar->current_frame + 1) % userCar->total_frames;
}

void updateText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor, SDL_Texture** texture, SDL_Rect* rect, const char* text)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    if (textSurface != NULL)
    {
        SDL_DestroyTexture(*texture); // Destroy the old texture first
        *texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        rect->w = textSurface->w;
        rect->h = textSurface->h;
        SDL_FreeSurface(textSurface);
    }
}

void cleanup(SDL_Surface* backgroundSurface, SDL_Texture* backgroundTexture, SDL_Texture* backgroundTexture2,
             SDL_Texture* scoreTexture, SDL_Texture* pauseTexture, SDL_Texture* vitesseTexture, SDL_Texture* highScoreTexture, TTF_Font* font, TTF_Font* gameOverFont,
             UserCar userCar, UserCar moto, SDL_Texture* obstacle[])
{
    // Free the loaded surface as it is no longer needed
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(backgroundTexture2);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(vitesseTexture);
    SDL_DestroyTexture(highScoreTexture);
    TTF_CloseFont(font);
    TTF_CloseFont(gameOverFont);  // Free gameOverFont

    // Destroy user car textures and moto texture
    SDL_DestroyTexture(moto.textures[0]);
    for(int i = 0; i < userCar.total_frames; ++i)
    {
        SDL_DestroyTexture(userCar.textures[i]);
    }

    // Destroy enemy car textures
    for(int i = 0; i < MAX_OBSTACLES; i++)
    {
        SDL_DestroyTexture(obstacle[i]);
    }

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void InitScore(SDL_Renderer* renderer, int* score, TTF_Font* font, SDL_Texture** scoreTexture, SDL_Rect *scoreRect, SDL_Color* textColor, int deplacementEffectue)
{
    if (deplacementEffectue)
    {
        // if 1 second has passed
        *score += 1;

        // update the score text
        char scoreText[20];
        sprintf(scoreText, "Score: %d", *score);
        updateText(renderer, font, *textColor, scoreTexture, scoreRect, scoreText);
    }
}

void initVitesse(UserCar* userCar, SDL_Renderer *renderer, SDL_Color *textColor, SDL_Texture** vitesseTexture, SDL_Rect *vitesseRect, TTF_Font* font)
{

    char vitesseText[20];
    sprintf(vitesseText, "Speed: %d km/h", userCar->velocity * 10);
    updateText(renderer, font, *textColor, vitesseTexture, vitesseRect, vitesseText);
}

int getHighScore()
{
    FILE *file = fopen("HighScore", "r");
    if (file == NULL)
    {
        printf("Could not open high score file\n");
        return -1;
    }

    char line[256];
    int highScore = 0;
    while (fgets(line, sizeof(line), file))
    {
        // Assume that the line format is always "Player : Name; Score : X"
        char *scoreStr = strstr(line, "Score : ");
        if (scoreStr != NULL)
        {
            highScore = atoi(scoreStr + 8);  // Skip past "Score : " to get the score
        }
    }

    fclose(file);
    return highScore;
}

int gameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score)
{
    SDL_Color greenColor = {0, 255, 0, 255}; // Define the color green.

    SDL_Surface* gameOverSurface = LoadImage("Images/GAME_OVER.png");
    SDL_Texture* gameOverTexture = LoadTexture(renderer, gameOverSurface);
    // Get dimensions of Game Over texture
    int gameOverWidth, gameOverHeight;
    SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameOverWidth, &gameOverHeight);
    // Position and size of Game Over texture
    SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - gameOverWidth / 2, 50, gameOverWidth, gameOverHeight};

    // Create the score text and get its dimensions
    char scoreText[50];
    sprintf(scoreText, "Score: %d", score);
    SDL_Texture* scoreTexture = createTextTexture(renderer, font, greenColor, scoreText);
    int scoreWidth, scoreHeight;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreWidth, &scoreHeight);
    // Position and size of Score text
    SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - scoreWidth / 2, gameOverHeight + 150, scoreWidth, scoreHeight};

    int highScore = getHighScore();
    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
    SDL_Texture* highScoreTexture = createTextTexture(renderer, font, greenColor, highScoreText);
    int highScoreWidth, highScoreHeight;
    SDL_QueryTexture(highScoreTexture, NULL, NULL, &highScoreWidth, &highScoreHeight);
    // Position and size of Highscore text
    SDL_Rect highScoreRect = {SCREEN_WIDTH / 2 - highScoreWidth / 2, gameOverHeight + 220, highScoreWidth, highScoreHeight};

    SDL_Texture* replayTexture = createTextTexture(renderer, font, greenColor, "Play Again");
    int replayWidth, replayHeight;
    SDL_QueryTexture(replayTexture, NULL, NULL, &replayWidth, &replayHeight);
    // Position and size of Replay text
    SDL_Rect replayRect = {SCREEN_WIDTH / 2 - replayWidth / 2, gameOverHeight + 290, replayWidth, replayHeight};

    SDL_Texture* quitTexture = createTextTexture(renderer, font, greenColor, "Quit");
    int quitWidth, quitHeight;
    SDL_QueryTexture(quitTexture, NULL, NULL, &quitWidth, &quitHeight);
    // Position and size of Quit text
    SDL_Rect quitRect = {SCREEN_WIDTH / 2 - quitWidth / 2, gameOverHeight + 360, quitWidth, quitHeight};

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
            {
                running = false;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x > replayRect.x && x < replayRect.x + replayRect.w && y > replayRect.y && y < replayRect.y + replayRect.h) {
                    // The user clicked on the "Play Again" button
                    SDL_DestroyTexture(gameOverTexture);
                    SDL_DestroyTexture(scoreTexture);
                    SDL_DestroyTexture(highScoreTexture);
                    SDL_DestroyTexture(replayTexture);
                    SDL_DestroyTexture(quitTexture);
                    return 1;
                }
                else if (x > quitRect.x && x < quitRect.x + quitRect.w && y > quitRect.y && y < quitRect.y + quitRect.h) {
                    // The user clicked on the "Quit" button
                    SDL_DestroyTexture(gameOverTexture);
                    SDL_DestroyTexture(scoreTexture);
                    SDL_DestroyTexture(highScoreTexture);
                    SDL_DestroyTexture(replayTexture);
                    SDL_DestroyTexture(quitTexture);
                    return 0;
                }
            }
        }

        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);
        SDL_RenderCopy(renderer, replayTexture, NULL, &replayRect);
        SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(highScoreTexture);
    SDL_DestroyTexture(replayTexture);
    SDL_DestroyTexture(quitTexture);

    return 0;
}


void LancerJeu(SDL_Renderer* renderer)
{
    // Initialise le générateur de nombres pseudo aléatoires
    srand(time(NULL));

    //Init jeu
    jeu j = initJeu();

    // Initialize SDL
    initializeSDL();

    // Initialize SDL_image
    initializeIMG();

    // Load the background image
    SDL_Surface *backgroundSurface = LoadImage("Images/Game.png");

    // Create a texture from the loaded surface
    SDL_Texture *backgroundTexture = LoadTexture(renderer, backgroundSurface);

    // Create a second texture from the same surface for scrolling background
    SDL_Texture *backgroundTexture2 = LoadTexture(renderer, backgroundSurface);

    initializeTTF();
    TTF_Font* font = loadFont("Font/arial_bold.ttf", 28);
    TTF_Font* gameOverFont = loadFont("Font/arial_bold.ttf", 38);

    SDL_Color textColor = {255, 255, 255, 255}; // white color
    SDL_Texture* scoreTexture = NULL;
    SDL_Texture* vitesseTexture = NULL;
    SDL_Rect scoreRect = {10, 10, 0, 0}; // position for score, top left
    SDL_Rect vitesseRect = {SCREEN_WIDTH - 260, 10, 0, 0}; // position for speed, top right

    // Create the texture for high score
    char highScoreText[30];
    sprintf(highScoreText, "High Score: %d", getHighScore());
    SDL_Texture* highScoreTexture = createTextTexture(renderer, font, textColor, highScoreText);
    int highScoreWidth, highScoreHeight;
    SDL_QueryTexture(highScoreTexture, NULL, NULL, &highScoreWidth, &highScoreHeight);
    SDL_Rect highScoreRect = {SCREEN_WIDTH / 2 - highScoreWidth / 2, 10, highScoreWidth, highScoreHeight}; // position for highscore, top middle

    // Initialisation de la voiture chasseur
    UserCar userCar = initVoiture(renderer, j.chasseur, 0);

    // Initialisation de la moto proie
    UserCar moto = initMoto(renderer, j.proie, 1);

    // Initialisation de la texture des obstacles
    initTexturesObstacles(renderer);

    int bgScroll = 0;  // Initialize background scroll offset

    int score = -1;

    bool isPaused = false;
    SDL_Texture* pauseTexture = NULL;
    SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100, 100}; // position for pause text, middle of the screen

    bool running = true;
    SDL_Event e;
    int deplacement = -2;
    int deplacementEffectue = 1;

    while (running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            deplacement = -2;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_p)
                {
                    isPaused = !isPaused; // Inverse l'état de pause
                    if (isPaused)
                    {
                        updateText(renderer, font, textColor, &pauseTexture, &pauseRect, "Paused");
                    }
                    else
                    {
                        SDL_DestroyTexture(pauseTexture);
                        pauseTexture = NULL;
                    }
                }
                if (!isPaused)
                {
                    if (e.key.keysym.sym == SDLK_LEFT)
                    {
                        deplacement = -1;
                    }
                    else if (e.key.keysym.sym == SDLK_RIGHT)
                    {
                        deplacement = 1;
                    }
                    else if (e.key.keysym.sym == SDLK_UP && !( SDL_GetModState() & KMOD_CTRL))
                    {
                        deplacement = 0;
                    }
                    else if (( SDL_GetModState() & KMOD_CTRL ) && e.key.keysym.sym == SDLK_UP)
                    {
                        if (userCar.velocity <= 19)
                        {
                            userCar.velocity += 1;
                            break;
                        }
                    }
                    else if (( SDL_GetModState() & KMOD_CTRL ) && e.key.keysym.sym == SDLK_DOWN)
                    {
                        if (userCar.velocity > 1)
                        {
                            userCar.velocity -= 1;
                            break;
                        }
                    }
                }
            }
        }

        if (deplacement != -2 && verifDeplacement(j.grille, deplacement, j.chasseur, 0))
        {
            running = !iterJeu(&j, deplacement);
            userCar.cell_x = j.chasseur;
            moto.cell_x = j.proie;
            deplacement = -2;
            deplacementEffectue = 1;
        }
        else if (deplacement != -2 && !verifDeplacement(j.grille, deplacement, j.chasseur, 0))
        {
            printf("Deplacement invalide\n");
            deplacement = -2;
        }

        if (!isPaused)
        {
            InitScore(renderer, &score, font, &scoreTexture, &scoreRect, &textColor, deplacementEffectue);
            deplacementEffectue = 0;

            initVitesse(&userCar, renderer, &textColor, &vitesseTexture, &vitesseRect, font);

            bgScroll += userCar.velocity;  // Scroll background at car's velocity

            // Reset scroll offset when it is greater than or equal to the height of the screen
            if (bgScroll >= SCREEN_HEIGHT)
            {
                bgScroll = 0;
            }
        }

        // Draw the scrolling backgrounds
        SDL_Rect bgQuad1 = { 0, bgScroll - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_Rect bgQuad2 = { 0, bgScroll, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgQuad1);
        SDL_RenderCopy(renderer, backgroundTexture2, NULL, &bgQuad2);

        drawMoto(renderer, &moto);
        drawVoiture(renderer, &userCar);

	    drawObstacles(renderer, j);

        // Draw the score and time
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(renderer, vitesseTexture, NULL, &vitesseRect);
        SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);

        // If game is paused, draw the pause message
        if (isPaused)
        {
            SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
        }

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

    }
    // Ajouter ceci après votre boucle de jeu
    int retry = gameOverScreen(renderer, gameOverFont, score);
    if (retry)
    {
        // Si le joueur veut rejouer, réexécuter la fonction LancerJeu
        LancerJeu(renderer);
    }

    cleanup(backgroundSurface, backgroundTexture, backgroundTexture2, scoreTexture, pauseTexture, vitesseTexture, highScoreTexture, font, gameOverFont, userCar, moto, obstacles);
}


/* int main() { */
/*     int seed = time(NULL); */
/*     seed = 1688484772; */
/*     srand(seed); */
/*     printf("%d\n", seed); */

/*     bot robot; */
/*     for (int i = 0; i < NB_REGLES; i++) { */
/* 	for (int j = 0; j <= 2; j++) { */
/* 	    robot[i][j] = (rand() % 4) + 1; */
/* 	} */
/* 	robot[i][3] = (rand() % 5) + 1; */
/* 	robot[i][4] = (rand() % 3) - 1; */
/* 	robot[i][5] = (rand() % 5) + 1; */
/*     } */
/*     // to do rajouter les jokers */

/*     printf("Bot aleatoire cree\n"); */
/*     printf("score de ce bot : %d\n", Jeu(robot)); */

/*     return 0; */
/* } */
