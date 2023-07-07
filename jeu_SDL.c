#include "jeu_SDL.h"
#include "Constantes.h"

SDL_Texture* obstacles[NOMBRE_SPRITE];
#define FRAME_DELAY 100  // définit le délai entre chaque frame en millisecondes

//Fonction pour initialiser le chasseur
UserCar initVoiture(SDL_Renderer *renderer, int x, int y)
{
    UserCar userCar;

    // Set up initial size and position of the user car
    userCar.cell_x = x;
    userCar.cell_y = 7 - y;
    // Initialize draw_x and draw_y for smooth animation
    userCar.draw_x = userCar.cell_x * ((double)TAILLE_CELLULE_LARGEUR/2) + 250;
    userCar.draw_y = userCar.cell_y * ((double)TAILLE_CELLULE_LONGUEUR/2);
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

    // Initialisez les valeurs de dessin avec les valeurs de cellule actuelles
    moto.draw_x = moto.cell_x * ((double)TAILLE_CELLULE_LARGEUR/2) + 285;
    moto.draw_y = moto.cell_y * ((double)TAILLE_CELLULE_LONGUEUR/2);

    // Chargez la texture pour la moto
    SDL_Surface *tempSurface = LoadImage("Images/Proie.png");

    moto.textures[0] = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return moto;
}

// Fonction pour dessiner la moto à l'écran
void drawMoto(SDL_Renderer *renderer, UserCar *moto)
{
    // Mettez à jour la position du rectangle en fonction des valeurs de dessin
    moto->rect.x = (int)moto->draw_x;
    moto->rect.y = (int)moto->draw_y;

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

void drawObstacles(SDL_Renderer *renderer, jeu g) {
    static int old_grille[NB_LIGNES][NB_COLONNES] = {0}; // Grille précédente
    static double draw_x[NB_LIGNES][NB_COLONNES] = {0}; // Positions de dessin réelles des obstacles
    static double draw_y[NB_LIGNES][NB_COLONNES] = {0}; // Positions de dessin réelles des obstacles

    for(int i = 0; i < NB_LIGNES; i++) {
        for(int j = 0; j < NB_COLONNES; j++) {
            if(g.grille[i][j]) {
                if(old_grille[i][j] != g.grille[i][j]) {
                    draw_x[i][j] = j * ((double)TAILLE_CELLULE_LARGEUR/2) + 250;
                    draw_y[i][j] = (7-i) * ((double)TAILLE_CELLULE_LONGUEUR/2);
                }

                // Mise à jour de la position de dessin en fonction de la vitesse
                double target_x = j * ((double)TAILLE_CELLULE_LARGEUR/2) + 250;
                double target_y = (7-i) * ((double)TAILLE_CELLULE_LONGUEUR/2);
                draw_x[i][j] += (target_x - draw_x[i][j]) * 0.04;
                draw_y[i][j] += (target_y - draw_y[i][j]) * 0.04;

                SDL_Rect posObstacle = {(int)draw_x[i][j], (int)draw_y[i][j], 140, 130};
                SDL_RenderCopy(renderer, obstacles[g.grille[i][j]-1], NULL, &posObstacle);

                old_grille[i][j] = g.grille[i][j];
            }
            else {
                if(old_grille[i][j] != 0) {
                    draw_x[i][j] = 0;
                    draw_y[i][j] = 0;
                    old_grille[i][j] = 0;
                }
            }
        }
    }
}

void drawVoiture(SDL_Renderer *renderer, UserCar *userCar)
{
    // Mettez à jour la position du rectangle en fonction des valeurs de dessin
    userCar->rect.x = (int)userCar->draw_x;
    userCar->rect.y = (int)userCar->draw_y;

    SDL_RenderCopy(renderer, userCar->textures[userCar->current_frame], NULL, &userCar->rect);

    // Obtenez le temps actuel en millisecondes
    Uint32 current_time = SDL_GetTicks();

    // Vérifiez si le délai entre chaque frame est écoulé
    if (current_time - userCar->last_frame_time >= FRAME_DELAY) {
        // Update the frame for the next draw
        userCar->current_frame = (userCar->current_frame + 1) % userCar->total_frames;

        // Mettez à jour le dernier temps de frame
        userCar->last_frame_time = current_time;
    }
}

void updateText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color textColor, SDL_Texture** texture, SDL_Rect* rect, const char* text)
{
    // If the string is empty, render a space instead
    const char* renderText = (text[0] != '\0') ? text : " ";

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, renderText, textColor);
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
             UserCar userCar, UserCar moto, SDL_Texture* obstacle[], Mix_Chunk* buttonSound, char *username)
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

    Mix_FreeChunk(buttonSound);
    if (username)
    {
        free(username);  // Libération de la mémoire
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
    FILE *file = fopen("DataLog/HighScore", "r");
    if (file == NULL)
    {
        printf("Could not open high score file\n");
        return -1;
    }

    char line[256];
    int highScore = INT_MAX; // Initialize to maximum integer value
    while (fgets(line, sizeof(line), file))
    {
        // Assume that the line format is always "Player: Name; Score: X" or "Bot: Name; Score: X"
        char *scoreStr = strstr(line, "Score: ");
        if (scoreStr != NULL)
        {
            char* end;
            long scoreLong = strtol(scoreStr + 7, &end, 10);

            if (end == scoreStr + 7)
            {
                printf("Error: could not convert score to an integer.\n");
                return -1;
            }

            if (scoreLong > INT_MAX || scoreLong < INT_MIN)
            {
                printf("Error: score is out of range for an integer.\n");
                return -1;
            }

            int score = (int)scoreLong;
            if(score < highScore)
            {
                // If the read score is less than the current highScore
                highScore = score;  // Update highScore
            }
        }
    }

    fclose(file);
    return highScore == INT_MAX ? 0 : highScore;  // Return 0 if no scores were found
}

void initHighScore()
{
    FILE *logFile, *highscoreFile;
    char line[MAX_LINE_LENGTH];
    char playerName[MAX_NAME_LENGTH], botName[MAX_NAME_LENGTH];
    int playerScore = INT_MAX, botScore = INT_MAX;
    char name[MAX_NAME_LENGTH];
    char scoreStr[MAX_NAME_LENGTH];
    long scoreLong;

    logFile = fopen("DataLog/GameLog", "r");
    if (logFile == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    while (fgets(line, sizeof(line), logFile))
    {
        sscanf(line, "%*[^:]: %[^;]; %*[^:]: %s", name, scoreStr);

        char *end;
        scoreLong = strtol(scoreStr, &end, 10);

        if (end == scoreStr)
        {
            printf("Error: could not convert score to an integer.\n");
            return;
        }

        if (scoreLong > INT_MAX || scoreLong < INT_MIN)
        {
            printf("Error: score is out of range for an integer.\n");
            return;
        }

        int score = (int)scoreLong;
        if (strstr(line, "Player"))
        {
            if (score < playerScore)
            {
                playerScore = score;
                strcpy(playerName, name);
            }
        }
        else
        {
            if (score < botScore)
            {
                botScore = score;
                strcpy(botName, name);
            }
        }
    }
    fclose(logFile);

    highscoreFile = fopen("DataLog/HighScore", "w");
    if (highscoreFile == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(highscoreFile, "Player: %s; Score: %d\n", playerName, playerScore);
    fprintf(highscoreFile, "Bot: %s; Score: %d\n", botName, botScore);
    fclose(highscoreFile);
}

void playButtonSound(Mix_Chunk* buttonSound)
{
    // Augmente le volume du son du bouton. La valeur doit être entre 0 et 128.
    Mix_VolumeChunk(buttonSound, 128); // met le volume au maximum
    // Play sound effect
    if(Mix_PlayChannel(-1, buttonSound, 0) == -1)
    {
        printf("Failed to play button sound! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

int gameOverScreen(SDL_Renderer* renderer, TTF_Font* font, int score, int highScore)
{
    SDL_Color greenColor = {0, 255, 0, 255}; // Define the color green.

    // Load the Game Over background image
    SDL_Surface* backgroundSurface = LoadImage("Images/Background_Game_Over.jpg");

    // Create a texture from the loaded surface
    SDL_Texture* backgroundTexture = LoadTexture(renderer, backgroundSurface);

    // Position and size of Background texture. In this case, we want to cover the whole screen.
    SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int gameOverWidth, gameOverHeight;
    SDL_Texture* gameOverTexture = NULL;

    // On charge l'image congratulation si l'utilisateur a un highscore
    if (score < highScore)
    {
        gameOverTexture = loadTexture(renderer, "Images/Congratulations.png");
    }
        // On charge l'image prey captured s'il n'a pas de highscore
    else if (score >= highScore)
    {
        gameOverTexture = loadTexture(renderer, "Images/Prey-Captured.png");
    }

    // Get dimensions of Game Over texture
    SDL_QueryTexture(gameOverTexture, NULL, NULL, &gameOverWidth, &gameOverHeight);
    // Position and size of Game Over texture
    SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - gameOverWidth / 2, 250, gameOverWidth, gameOverHeight};

    // Create the score text and get its dimensions
    char scoreText[50];
    if (score < highScore)
    {
        sprintf(scoreText, "New High Score: %d", score);
    }
    else if (score >= highScore)
    {
        sprintf(scoreText, "Score: %d", score);
    }
    SDL_Texture* scoreTexture = createTextTexture(renderer, font, greenColor, scoreText);
    int scoreWidth, scoreHeight;
    SDL_QueryTexture(scoreTexture, NULL, NULL, &scoreWidth, &scoreHeight);
    // Position and size of Score text
    SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - scoreWidth / 2, gameOverHeight + 250, scoreWidth, scoreHeight};

    char highScoreText[50];
    sprintf(highScoreText, "High Score: %d", highScore);
    SDL_Texture* highScoreTexture = createTextTexture(renderer, font, greenColor, highScoreText);
    int highScoreWidth, highScoreHeight;
    SDL_QueryTexture(highScoreTexture, NULL, NULL, &highScoreWidth, &highScoreHeight);
    // Position and size of Highscore text
    SDL_Rect highScoreRect = {SCREEN_WIDTH / 2 - highScoreWidth / 2, gameOverHeight + 320, highScoreWidth, highScoreHeight};

    SDL_Texture* replayTexture = createTextTexture(renderer, font, greenColor, "Play Again");
    int replayWidth, replayHeight;
    SDL_QueryTexture(replayTexture, NULL, NULL, &replayWidth, &replayHeight);
    // Position and size of Replay text
    SDL_Rect replayRect = {SCREEN_WIDTH / 2 - replayWidth / 2, gameOverHeight + 390, replayWidth, replayHeight};

    SDL_Texture* quitTexture = createTextTexture(renderer, font, greenColor, "Quit");
    int quitWidth, quitHeight;
    SDL_QueryTexture(quitTexture, NULL, NULL, &quitWidth, &quitHeight);
    // Position and size of Quit text
    SDL_Rect quitRect = {SCREEN_WIDTH / 2 - quitWidth / 2, gameOverHeight + 460, quitWidth, quitHeight};

    SDL_Color WHITE = {255, 255, 255, 255}; // On hover change color to white
    Uint32 startTime = SDL_GetTicks(); // This gets the number of milliseconds since the SDL library was initialized.

    bool running = true;

    // Frame rate
    const int FPS = 120;
    Uint32 frameDelay = 1000 / FPS;

    Uint32 frameStart;
    Uint32 frameTime;

    while (running)
    {
        frameStart = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN))
            {
                running = false;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x > replayRect.x && x < replayRect.x + replayRect.w && y > replayRect.y && y < replayRect.y + replayRect.h)
                {
                    // The user clicked on the "Play Again" button
                    SDL_DestroyTexture(gameOverTexture);
                    SDL_DestroyTexture(scoreTexture);
                    SDL_DestroyTexture(highScoreTexture);
                    SDL_DestroyTexture(replayTexture);
                    SDL_DestroyTexture(quitTexture);
                    return 1;
                }
                else if (x > quitRect.x && x < quitRect.x + quitRect.w && y > quitRect.y && y < quitRect.y + quitRect.h)
                {
                    // The user clicked on the "Quit" button
                    SDL_DestroyTexture(gameOverTexture);
                    SDL_DestroyTexture(scoreTexture);
                    SDL_DestroyTexture(highScoreTexture);
                    SDL_DestroyTexture(replayTexture);
                    SDL_DestroyTexture(quitTexture);
                    return 0;
                }
            }
            else if (e.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x > replayRect.x && x < replayRect.x + replayRect.w && y > replayRect.y && y < replayRect.y + replayRect.h)
                {
                    SDL_DestroyTexture(replayTexture);
                    replayTexture = createTextTexture(renderer, font, WHITE, "Play Again");
                }
                else
                {
                    SDL_DestroyTexture(replayTexture);
                    replayTexture = createTextTexture(renderer, font, greenColor, "Play Again");
                }

                if (x > quitRect.x && x < quitRect.x + quitRect.w && y > quitRect.y && y < quitRect.y + quitRect.h)
                {
                    SDL_DestroyTexture(quitTexture);
                    quitTexture = createTextTexture(renderer, font, WHITE, "Quit");
                }
                else
                {
                    SDL_DestroyTexture(quitTexture);
                    quitTexture = createTextTexture(renderer, font, greenColor, "Quit");
                }
            }
        }

        if (score < highScore)
        {
            Uint32 elapsedTime = SDL_GetTicks() - startTime;

            if ((elapsedTime / 500) % 2 == 0)  // We're dividing by 500 to convert milliseconds to half-seconds.
            {
                SDL_DestroyTexture(scoreTexture); // Destroy the previous texture
                scoreTexture = createTextTexture(renderer, font, WHITE, scoreText);
            }
            else
            {
                SDL_DestroyTexture(scoreTexture); // Destroy the previous texture
                scoreTexture = createTextTexture(renderer, font, greenColor, scoreText);
            }
        }

        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);
        SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(renderer, highScoreTexture, NULL, &highScoreRect);
        SDL_RenderCopy(renderer, replayTexture, NULL, &replayRect);
        SDL_RenderCopy(renderer, quitTexture, NULL, &quitRect);

        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(highScoreTexture);
    SDL_DestroyTexture(replayTexture);
    SDL_DestroyTexture(quitTexture);
    SDL_DestroyTexture(backgroundTexture);

    return 0;
}

char* DemanderQqch(SDL_Renderer* renderer, int *QuitterJeu, char *pathBackgroud, char *pathMessage)
{
    initializeTTF();
    TTF_Font* font = loadFont("Font/arial_bold.ttf", 32);
    TTF_Font* font_button = loadFont("Font/arial_bold.ttf", 35);

    // Load button sound
    Mix_Chunk* buttonSound = Mix_LoadWAV("Musiques/Mouse_Click.wav");
    if(buttonSound == NULL)
    {
        printf("Failed to load button sound! SDL_mixer Error: %s\n", Mix_GetError());
    }

    SDL_Color WHITE = {255, 255, 255, 255};
    SDL_Color BLACK = {0, 0, 0, 255};
    SDL_Color HOVER_COLOR = {255, 0, 0, 255};
    SDL_Color BUTTON_COLOR = {100, 200, 100, 255};

    SDL_Texture *backgroundTexture = loadTexture(renderer, pathBackgroud);
    // Dim the background image by half
    SDL_SetTextureColorMod(backgroundTexture, 100, 100, 100);

    // Image qui demande a l'utilisateur d'entrer son nom
    SDL_Texture* promptTexture = loadTexture(renderer, pathMessage);
    int prompt_width, prompt_height;
    SDL_QueryTexture(promptTexture, NULL, NULL, &prompt_width, &prompt_height);
    SDL_Rect promptRect = {SCREEN_WIDTH / 2 - prompt_width / 2, SCREEN_HEIGHT / 2 - prompt_height - 50, prompt_width, prompt_height};

    SDL_Texture* usernameTexture = NULL;
    SDL_Rect usernameRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2, 500, 40};

    SDL_Texture* validateTexture = createTextTexture(renderer, font_button, WHITE, "Valider");
    SDL_Texture* validateHoverTexture = createTextTexture(renderer, font_button, HOVER_COLOR, "Valider");

    int validate_width, validate_height;
    SDL_QueryTexture(validateTexture, NULL, NULL, &validate_width, &validate_height);
    SDL_Rect validateRect = {SCREEN_WIDTH / 2 - validate_width / 2, SCREEN_HEIGHT / 2 + 100, validate_width, validate_height};
    SDL_Rect validateBorderRect = {validateRect.x - 5, validateRect.y - 5, validateRect.w + 10, validateRect.h + 10};

    SDL_Rect textBoxRect = {SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2, 500, 50};

    bool running = true;
    SDL_Event e;
    bool buttonHovered = false;

    char username[29] = "";

    // Frame rate
    const int FPS = 120;
    Uint32 frameDelay = 1000 / FPS;

    Uint32 frameStart;
    Uint32 frameTime;

    while (running)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0)
        {
            if ( (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) || (e.type == SDL_QUIT) )
            {
                running = false;
                *QuitterJeu = 1;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_RETURN)
                {
                    playButtonSound(buttonSound);
                    running = false;
                }
                else if (e.key.keysym.sym == SDLK_BACKSPACE)
                {
                    if (strlen(username) > 0)
                    {
                        username[strlen(username) - 1] = '\0';
                    }
                }
                else if (e.key.keysym.sym >= SDLK_SPACE && e.key.keysym.sym <= SDLK_z)
                {
                    if(e.key.keysym.sym <= 126) // Check if the value is within ASCII range
                    {
                        char ch[2] = { (char)e.key.keysym.sym, '\0' };
                        if ((e.key.keysym.mod & KMOD_SHIFT) || (SDL_GetModState() & KMOD_CAPS))
                        {
                            ch[0] = (char)toupper(ch[0]);
                        }
                        strncat(username, ch, 28 - strlen(username));
                    }
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= validateRect.x && x <= validateRect.x + validateRect.w &&
                    y >= validateRect.y && y <= validateRect.y + validateRect.h)
                {
                    playButtonSound(buttonSound);
                    // Clicked on "Validate" button
                    running = false;
                }
            }
            else if (e.type ==SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= validateRect.x && x <= validateRect.x + validateRect.w &&
                    y >= validateRect.y && y <= validateRect.y + validateRect.h)
                {
                    // Mouse is over "Validate" button
                    buttonHovered = true;
                }
                else
                {
                    // Mouse is not over "Validate" button
                    buttonHovered = false;
                }
            }
        }

        updateText(renderer, font, BLACK, &usernameTexture, &usernameRect, username);

        //Render the background image
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        // Render the prompt
        SDL_RenderCopy(renderer, promptTexture, NULL, &promptRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &textBoxRect);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderCopy(renderer, usernameTexture, NULL, &usernameRect);

        if (buttonHovered)
        {
            SDL_SetRenderDrawColor(renderer, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b, BUTTON_COLOR.a);
            SDL_RenderFillRect(renderer, &validateBorderRect);
            SDL_RenderCopy(renderer, validateHoverTexture, NULL, &validateRect);
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, BUTTON_COLOR.r, BUTTON_COLOR.g, BUTTON_COLOR.b, BUTTON_COLOR.a);
            SDL_RenderFillRect(renderer, &validateBorderRect);
            SDL_RenderCopy(renderer, validateTexture, NULL, &validateRect);
        }

        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyTexture(usernameTexture);
    SDL_DestroyTexture(validateTexture);
    SDL_DestroyTexture(validateHoverTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(promptTexture);
    TTF_CloseFont(font);
    TTF_CloseFont(font_button);
    //Mix_FreeChunk(buttonSound); le libere trop rapidement et donc on entend pas le son (on devrait mettre un sdl delay mais pas efficace)
    return strdup(username);
}

void logScore(const char* username, int score, bool humain)
{
    FILE *file = fopen("DataLog/GameLog", "a");
    if (file != NULL)
    {
        if(humain)
            fprintf(file, "Player : %s; Score : %d\n", username, score);
        else
            fprintf(file, "Bot : %s; Score : %d\n", username, score);
        fclose(file);
    }
    else
    {
        printf("Failed to open the file.\n");
    }
}

void LancerJeu(SDL_Renderer* renderer, bot robot, char * botname)
{
    bool humain = (robot == NULL);

    int quitterJeu = 0;

    //recuperer le nom du joueur/bot
    char *username;

    if(humain) {
        char *pathBackgroud = "Images/DemanderUsername_Background.jpg";
        char *pathMessage = "Images/Entrer-votre-nom.png";
        username = DemanderQqch(renderer, &quitterJeu, pathBackgroud, pathMessage);
    } else {
        username = botname;
    }

    if (!quitterJeu)
    {
        // Load button sound
        Mix_Chunk* buttonSound2 = Mix_LoadWAV("Musiques/Error_Click.wav");
        if(buttonSound2 == NULL)
        {
            printf("Failed to load button sound! SDL_mixer Error: %s\n", Mix_GetError());
        }
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

        int situation[TAILLE_ETAT];
        int frames = 191;

        // Frame rate
        const int FPS = 120;
        Uint32 frameDelay = 1000 / FPS;

        Uint32 frameStart;
        Uint32 frameTime;

        while (running)
        {
            frameStart = SDL_GetTicks();

            while (SDL_PollEvent(&e) != 0)
            {
                deplacement = -2;

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                    quitterJeu = 1;
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
                            if(humain) deplacement = -1;
                        }
                        else if (e.key.keysym.sym == SDLK_RIGHT)
                        {
                            if(humain) deplacement = 1;
                        }
                        else if (e.key.keysym.sym == SDLK_UP && !( SDL_GetModState() & KMOD_CTRL))
                        {
                            if(humain) deplacement = 0;
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

            frames--;

            if(!humain && frames == 0) {
                getSituationFromJeu(j, situation);
                deplacement = deplacementFromBot(j, robot, situation);
                frames = -9 * userCar.velocity + 200;
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
                playButtonSound(buttonSound2);
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

            double target_x = userCar.cell_x * ((double)TAILLE_CELLULE_LARGEUR/2) + 250;
            double target_y = userCar.cell_y * ((double)TAILLE_CELLULE_LONGUEUR/2);
            userCar.draw_x += (target_x - userCar.draw_x) * 0.05;
            userCar.draw_y += (target_y - userCar.draw_y) * 0.05;

            target_x = moto.cell_x * ((double)TAILLE_CELLULE_LARGEUR/2) + 285;
            target_y = moto.cell_y * ((double)TAILLE_CELLULE_LONGUEUR/2);
            moto.draw_x += (target_x - moto.draw_x) * 0.05;
            moto.draw_y += (target_y - moto.draw_y) * 0.05;

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


            frameTime = SDL_GetTicks() - frameStart;

            if(frameDelay > frameTime)
            {
                SDL_Delay(frameDelay - frameTime);
            }

        }

        if (!quitterJeu)
        {
            // Enregistrer le nom et le score du joueur
            int highscore_precedent = getHighScore();
            logScore(username, score, humain);
            initHighScore();

            // Ajouter ceci après votre boucle de jeu
            int retry = gameOverScreen(renderer, gameOverFont, score, highscore_precedent);
            if (retry)
            {
                // Si le joueur veut rejouer, réexécuter la fonction LancerJeu
                LancerJeu(renderer, robot, botname);
            }
        }
        cleanup(backgroundSurface, backgroundTexture, backgroundTexture2, scoreTexture, pauseTexture, vitesseTexture, highScoreTexture, font, gameOverFont, userCar, moto, obstacles, buttonSound2, username);
    }
}
