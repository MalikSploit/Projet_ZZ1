#include "Menu.h"
#include "SDL_Initialisation.h"
#include "jeu_SDL.h"


void initHighScore()
{
    FILE *logFile, *highscoreFile;
    char line[MAX_LINE_LENGTH];
    char playerName[MAX_NAME_LENGTH], botName[MAX_NAME_LENGTH];
    int playerScore = INT_MAX, botScore = INT_MAX;
    char name[MAX_NAME_LENGTH];
    char scoreStr[MAX_NAME_LENGTH];
    long scoreLong;

    logFile = fopen("GameLog", "r");
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
                strcpy(botName, "Robocop");
            }
        }
    }
    fclose(logFile);

    highscoreFile = fopen("HighScore", "w");
    if (highscoreFile == NULL)
    {
        printf("Error opening file\n");
        return;
    }

    fprintf(highscoreFile, "Player: %s; Score: %d\n", playerName, playerScore);
    fprintf(highscoreFile, "Bot: %s; Score: %d\n", botName, botScore);
    fclose(highscoreFile);
}

void calculerMoyenneScores()
{
    FILE* fichier = fopen("GameLog", "r"); // Ouvrir le fichier de log en mode lecture
    char ligne[256];
    int totalScoreJoueur = 0, totalScoreBot = 0, countJoueur = 0, countBot = 0;

    while (fgets(ligne, sizeof ligne, fichier))
    {
        char* nom = strtok(ligne, ";");
        char* scoreStr = strtok(NULL, ";");

        // Convertir le score en int, on saute les 8 premiers caractères " Score : "
        char* end;
        long scoreLong = strtol(scoreStr + 8, &end, 10);
        if (end == scoreStr + 8)
        {
            printf("Error: could not convert score to an integer.\n");
            return;
        }
        if (scoreLong > INT_MAX || scoreLong < INT_MIN)
        {
            printf("Error: score is out of range for an integer.\n");
            return;
        }

        int score = (int) scoreLong;

        if (strstr(nom, "Player"))
        {
            totalScoreJoueur += score;
            countJoueur++;
        }
        else if (strstr(nom, "Bot"))
        {
            totalScoreBot += score;
            countBot++;
        }
    }
    fclose(fichier); // Fermer le fichier de log

    double moyenneScoreJoueur = (double) totalScoreJoueur / countJoueur;
    double moyenneScoreBot = (double) totalScoreBot / countBot;

    // Maintenant, nous allons écrire les moyennes dans un nouveau fichier
    FILE* fichierMoyennes = fopen("moyennes", "w"); // Ouvrir le fichier de moyennes en mode écriture
    fprintf(fichierMoyennes, "Moyenne du score du joueur : %.2f\n", moyenneScoreJoueur);
    fprintf(fichierMoyennes, "Moyenne du score du bot : %.2f\n", moyenneScoreBot);
    fclose(fichierMoyennes); // Fermer le fichier de moyennes
}

void displayHelp(SDL_Renderer* renderer)
{
    // Load a font
    TTF_Font* font = TTF_OpenFont("Font/arial_bold.ttf", 29);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *backgroundTexture = loadTexture(renderer, "Images/Help_Background.jpg");

    // Set the color for the text
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Image instructions
    SDL_Texture* promptTexture = loadTexture(renderer, "Images/Instructions.png");
    int prompt_width, prompt_height;
    SDL_QueryTexture(promptTexture, NULL, NULL, &prompt_width, &prompt_height);
    SDL_Rect promptRect = {SCREEN_WIDTH / 2 - prompt_width / 2, 50, prompt_width, prompt_height};


    // Set the text for the instructions
    const char* instructions[] = {
            "Les extraterrestres se deplacent a la fois horizontalement et verticalement",
            "s'approchant du canon, quand ils arrivant au cote droit ou gauche.",
            "Le canon peut etre controle pour tirer des lasers afin de",
            "detruire les extraterrestres, tandis que les extraterrestres",
            "tireront au hasard sur le canon.",
            "Si un extraterrestre est abattu par le canon, il est detruit",
            "si le canon est touche, une vie est perdue.",
            "Utiliser les fleches droites et gauches pour se deplacer.",
            "Appuyer sur le bouton espace pour tirer",
    };
    int numLines = sizeof(instructions) / sizeof(instructions[0]);

    // Create a texture for each line of the instructions
    SDL_Texture** textures = (SDL_Texture**) malloc(numLines * sizeof(SDL_Texture*));
    if (textures == NULL) {
        printf("Failed to allocate memory for textures!\n");
        return;
    }
    for (int i = 0; i < numLines; i++)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(font, instructions[i], textColor);
        textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    // Clear the renderer
    SDL_RenderClear(renderer);

    // Render the background and the image instructions
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_RenderCopy(renderer, promptTexture, NULL, &promptRect);

    // Copy the textures to the renderer
    SDL_Rect textRect;
    textRect.x = 400;
    textRect.y = 300;
    for (int i = 0; i < numLines; i++)
    {
        SDL_QueryTexture(textures[i], NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, textures[i], NULL, &textRect);
        textRect.y += textRect.h + 5; // Move down for the next line (adjust the value to increase or decrease the line spacing)
    }

    // Show the renderer contents on the screen
    SDL_RenderPresent(renderer);
    // Event handler
    SDL_Event e;

    // Wait for the user to close the help screen
    bool closeHelp = false;
    while (!closeHelp)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN != 0 || (e.type == SDL_MOUSEBUTTONDOWN))
            {
                closeHelp = true;
            }
        }
    }

    // Clean up
    for (int i = 0; i < numLines; i++)
    {
        SDL_DestroyTexture(textures[i]);
    }
    free(textures);
    TTF_CloseFont(font);
    SDL_DestroyTexture(backgroundTexture);
}


void displayHighScore(SDL_Renderer* renderer)
{
    FILE *file = fopen("HighScore", "r");
    if (file == NULL) {
        printf("Could not open HighScore file!\n");
        return;
    }

    HighScores highScores;
    highScores.playerScore = INT_MAX;
    highScores.botScore = INT_MAX;

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char name[MAX_NAME_LENGTH];
        char scoreStr[MAX_NAME_LENGTH];
        sscanf(line, "%*[^:]: %[^;]; %*[^:]: %s", name, scoreStr);

        char* end;
        long scoreLong = strtol(scoreStr, &end, 10);

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
            strcpy(highScores.playerName, name);
            highScores.playerScore = score;
        }
        else
        {  // Bot
            strcpy(highScores.botName, name);
            highScores.botScore = score;
        }
    }

    fclose(file);

    double averagePlayerScore, averageBotScore;
    char buffer[256];
    FILE *averageFile = fopen("moyennes", "r");
    if (averageFile != NULL)
    {
        // Read player's average score
        fgets(buffer, sizeof(buffer), averageFile);
        averagePlayerScore = strtod(strchr(buffer, ':') + 2, NULL);
        if (averagePlayerScore == 0)
        {
            printf("Error: could not convert player's average score to a float.\n");
            return;
        }

        // Read bot's average score
        fgets(buffer, sizeof(buffer), averageFile);
        averageBotScore = strtod(strchr(buffer, ':') + 2, NULL);
        if (averageBotScore == 0)
        {
            printf("Error: could not convert bot's average score to a float.\n");
            return;
        }
    }
    else
    {
        printf("Could not open averages file!\n");
        return;
    }

    // Load a font
    TTF_Font* font = TTF_OpenFont("Font/arial_bold.ttf", 24);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create the high score and average score strings
    char highScorePlayerText[100], highScoreBotText[100];
    char averagePlayerText[100], averageBotText[100];
    sprintf(highScorePlayerText, "Best player score : %s with %d points", highScores.playerName, highScores.playerScore);
    sprintf(highScoreBotText, "Best bot score : %s with %d points", highScores.botName, highScores.botScore);
    sprintf(averagePlayerText, "Average player score: %.2f points", averagePlayerScore);
    sprintf(averageBotText, "Average bot score: %.2f points", averageBotScore);

    // Render the high scores
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    SDL_Surface* surfacePlayer = TTF_RenderText_Blended(font, highScorePlayerText, textColor);
    SDL_Surface* surfaceBot = TTF_RenderText_Blended(font, highScoreBotText, textColor);
    SDL_Texture* texturePlayer = SDL_CreateTextureFromSurface(renderer, surfacePlayer);
    SDL_Texture* textureBot = SDL_CreateTextureFromSurface(renderer, surfaceBot);
    SDL_FreeSurface(surfacePlayer);
    SDL_FreeSurface(surfaceBot);

    // Render the Average score
    SDL_Surface* surfacePlayerAvg = TTF_RenderText_Blended(font, averagePlayerText, textColor);
    SDL_Surface* surfaceBotAvg = TTF_RenderText_Blended(font, averageBotText, textColor);
    SDL_Texture* texturePlayerAvg = SDL_CreateTextureFromSurface(renderer, surfacePlayerAvg);
    SDL_Texture* textureBotAvg = SDL_CreateTextureFromSurface(renderer, surfaceBotAvg);
    SDL_FreeSurface(surfacePlayerAvg);
    SDL_FreeSurface(surfaceBotAvg);

    SDL_Rect textRectPlayerAvg, textRectBotAvg;
    SDL_QueryTexture(texturePlayerAvg, NULL, NULL, &textRectPlayerAvg.w, &textRectPlayerAvg.h);
    SDL_QueryTexture(textureBotAvg, NULL, NULL, &textRectBotAvg.w, &textRectBotAvg.h);
    textRectPlayerAvg.x = (SCREEN_WIDTH - textRectPlayerAvg.w) / 2; // Calculate the x coordinate to center the text
    textRectBotAvg.x = (SCREEN_WIDTH - textRectBotAvg.w) / 2; // Calculate the x coordinate to center the text
    textRectPlayerAvg.y = 150; // Position these lower on the screen
    textRectBotAvg.y = 200;

    SDL_Rect textRectPlayer, textRectBot;
    SDL_QueryTexture(texturePlayer, NULL, NULL, &textRectPlayer.w, &textRectPlayer.h);
    SDL_QueryTexture(textureBot, NULL, NULL, &textRectBot.w, &textRectBot.h);
    textRectPlayer.x = (SCREEN_WIDTH - textRectPlayer.w) / 2; // Calculate the x coordinate to center the text
    textRectBot.x = (SCREEN_WIDTH - textRectBot.w) / 2; // Calculate the x coordinate to center the text
    textRectPlayer.y = 50;
    textRectBot.y = 100;

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Copy the texture with the high scores to the renderer
    SDL_RenderCopy(renderer, texturePlayer, NULL, &textRectPlayer);
    SDL_RenderCopy(renderer, textureBot, NULL, &textRectBot);
    // Copy the texture with the average scores to the renderer
    SDL_RenderCopy(renderer, texturePlayerAvg, NULL, &textRectPlayerAvg);
    SDL_RenderCopy(renderer, textureBotAvg, NULL, &textRectBotAvg);

    // Show the renderer contents on the screen
    SDL_RenderPresent(renderer);

    // Wait until user presses a key
    bool highScoreOpen = true;
    SDL_Event e;
    while (highScoreOpen)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN != 0 || (e.type == SDL_MOUSEBUTTONDOWN))
            {
                highScoreOpen = false;
            }
        }
    }

    // Clean up
    SDL_DestroyTexture(texturePlayer);
    SDL_DestroyTexture(textureBot);
    SDL_DestroyTexture(texturePlayerAvg);
    SDL_DestroyTexture(textureBotAvg);
    TTF_CloseFont(font);
}


void drawButton(SDL_Renderer* renderer, Button* button)
{
    // Render the button's text
    int textWidth, textHeight;
    SDL_QueryTexture(button->texture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect renderQuad = { button->rect.x + (button->rect.w - textWidth) / 2,
                            button->rect.y + (button->rect.h - textHeight) / 2,
                            textWidth, textHeight };
    SDL_RenderCopy(renderer, button->texture, NULL, &renderQuad);
}

int main()
{
    if (initializeSDL() == -1)
    {
        return 1;
    }
    if (initializeTTF() == -1)
    {
        return 1;
    }
    if (initializeIMG() == -1)
    {
        return 1;
    }
    SDL_Window* window = createWindow("Highway Racer");
    if (window == NULL)
    {
        return 1;
    }
    SDL_Renderer* renderer = createRenderer(window);
    if (renderer == NULL)
    {
        return 1;
    }
    SDL_Texture* logo1 = loadLogo(renderer, "Images/Highway-Racer.png");
    if (logo1 == NULL)
    {
        return 1;
    }
    SDL_Texture* logo2 = loadLogo(renderer, "Images/Menu_Image.jpg");
    if (logo2 == NULL)
    {
        return 1;
    }
    SDL_Texture* bgTexture1 = loadLogo(renderer, "Images/Background_Menu.png");
    SDL_Texture* bgTexture2 = loadLogo(renderer, "Images/Background_Menu.png");
    if (bgTexture1 == NULL || bgTexture2 == NULL)
    {
        return 1;
    }
    TTF_Font* font = loadFont("Font/arial_bold.ttf", 38);
    if (font == NULL)
    {
        return 1;
    }

    int logo1Width, logo1Height;
    SDL_QueryTexture(logo1, NULL, NULL, &logo1Width, &logo1Height);

    int logo2Width, logo2Height;
    SDL_QueryTexture(logo2, NULL, NULL, &logo2Width, &logo2Height);

    SDL_Rect bgRect1 = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect bgRect2 = { SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

    // Define buttons
    SDL_Color WHITE = {255, 255, 255, 255}; // On hover change color to white
    Button buttons[5];
    for (int i = 0; i < 5; ++i)
    {
        buttons[i].rect.x = SCREEN_WIDTH / 2 - 50;
        buttons[i].rect.y = SCREEN_HEIGHT / 2 + 70 * i + 140;
        buttons[i].rect.w = 100;
        buttons[i].rect.h = 50;
        buttons[i].color.r = 0;
        buttons[i].color.g = 255;
        buttons[i].color.b = 0;
        buttons[i].color.a = 255;
        buttons[i].font = font;
    }
    buttons[0].text = "New Game";
    buttons[1].text = "Simulation";
    buttons[2].text = "Scoreboard";
    buttons[3].text = "Help";
    buttons[4].text = "Quit";

    for (int i = 0; i < 5; ++i)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(font, buttons[i].text, buttons[i].color);
        buttons[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    // Initialisation du high score a partir de GameLog
    initHighScore();

    // On calcule la moyenne du score des joueurs et des bots
    calculerMoyenneScores();

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Compteur de temps pour l'animation
    double time = 0.0;

    // While application is running
    while (!quit)
    {
        // Calcul de la nouvelle vitesse
        double amplitude = 3.0 + 2.0 * sin(0.1 * time);  // Amplitude entre 1 et 5
        int bgSpeed = (int)(amplitude * sin(time)) + 1;  // Vitesse variable

        // La vitesse de l'animation dépend du delta
        time += 0.01;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the background
        SDL_RenderCopy(renderer, bgTexture1, NULL, &bgRect1);
        SDL_RenderCopy(renderer, bgTexture2, NULL, &bgRect2);

        // Move the background
        bgRect1.x -= bgSpeed;
        bgRect2.x -= bgSpeed;

        // Check if the background has completely scrolled off the screen
        if (bgRect1.x + SCREEN_WIDTH < 0) bgRect1.x = SCREEN_WIDTH;
        if (bgRect2.x + SCREEN_WIDTH < 0) bgRect2.x = SCREEN_WIDTH;

        // Draw logo1 (High_Racer Text)
        SDL_Rect logo1Quad = { SCREEN_WIDTH / 2 - logo1Width / 2, 40, logo1Width, logo1Height };
        SDL_RenderCopy(renderer, logo1, NULL, &logo1Quad);

        // Define the new dimensions
        int newLogo2Width = logo2Width / 2;
        double newLogo2Height = logo2Height / 2.3;

        // Draw logo2 (Menu Image)
        SDL_Rect logo2Quad = { SCREEN_WIDTH / 2 - newLogo2Width / 2, 20 + logo1Height, newLogo2Width, (int)newLogo2Height };
        SDL_RenderCopy(renderer, logo2, NULL, &logo2Quad);

        // Draw buttons
        for (int i = 0; i < 5; ++i)
        {
            drawButton(renderer, &buttons[i]);
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window)) || ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)))
            {
                quit = true;
            }
            // User hovers above the menu buttons
            if (e.type == SDL_MOUSEMOTION)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                for (int i = 0; i < 5; ++i)
                {
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &(buttons[i].rect)))
                    {

                        // Change button text color and recreate the texture
                        SDL_Surface* surface = TTF_RenderText_Blended(buttons[i].font, buttons[i].text, WHITE);
                        SDL_DestroyTexture(buttons[i].texture); // Destroy the old texture first
                        buttons[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_FreeSurface(surface);

                        // Redraw the button
                        drawButton(renderer, &buttons[i]);
                        SDL_RenderPresent(renderer);
                    }
                    else
                    {
                        // If button is not clicked, reset its color to the original one
                        SDL_Surface* surface = TTF_RenderText_Blended(buttons[i].font, buttons[i].text, buttons[i].color);
                        SDL_DestroyTexture(buttons[i].texture); // Destroy the old texture first
                        buttons[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
                        SDL_FreeSurface(surface);
                    }
                }

            }
            // User clicks the mouse
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                for (int i = 0; i < 5; ++i)
                {
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &(buttons[i].rect)))
                    {
                        if (strcmp(buttons[i].text, "Help") == 0)
                        {
                            SDL_RenderClear(renderer); // Clear the screen
                            displayHelp(renderer);
                        }
                        if (strcmp(buttons[i].text, "Quit") == 0)
                        {
                            quit = true;
                            break;
                        }
                        if (strcmp(buttons[i].text, "Scoreboard") == 0)
                        {
                            SDL_RenderClear(renderer); // Clear the screen
                            displayHighScore(renderer);
                        }
                        if (strcmp(buttons[i].text, "Simulation") == 0)
                        {

                        }
                        if (strcmp(buttons[i].text, "New Game") == 0)
                        {
                            SDL_RenderClear(renderer); // Clear the screen
                            LancerJeu(renderer);
                            quit = true;
                        }
                    }
                }
            }
        }
    }

    // Free resources and close SDL
    SDL_DestroyTexture(logo1);  // destroy the logo1 texture
    SDL_DestroyTexture(logo2);  // destroy the logo2 texture
    SDL_DestroyTexture(bgTexture1);
    SDL_DestroyTexture(bgTexture2);

    for (int i = 0; i < 5; ++i)
    {
        SDL_DestroyTexture(buttons[i].texture);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();  // quit SDL_image
    SDL_Quit();

    return 0;
}
