#include "Menu.h"

void calculerMoyenneScores()
{
    FILE* fichier = fopen("DataLog/GameLog", "r"); // Ouvrir le fichier de log en mode lecture
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
    FILE* fichierMoyennes = fopen("DataLog/moyennes", "w"); // Ouvrir le fichier de moyennes en mode écriture
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
            "Le voleur vient de s'echapper en moto !",
            "",
            "Rattrapez-le en le bloquant avec votre voiture de police.",
            "Pour rattraper la moto, vous devez reussir a la bloquer avec le moins de",
            "coups possibles.",
            "Pour ce faire, vous devez reussir a placer votre voiture derriere la ",
            "moto lorsqu'elle est encerclee.",
            "",
            "Le jeu se joue au tour par tour avec les fleches directionnelles",
            "(gauche, droit, et haut).",
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

    // Frame rate
    const int FPS = 120;
    Uint32 frameDelay = 1000 / FPS;
    
    Uint32 frameStart;
    Uint32 frameTime;

    while (!closeHelp)
    {
        frameStart = SDL_GetTicks();
	
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN  || (e.type == SDL_MOUSEBUTTONDOWN))
            {
                closeHelp = true;
            }
        }
        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
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

void displayScoreboard(SDL_Renderer* renderer)
{
    FILE *file = fopen("DataLog/HighScore", "r");
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
    FILE *averageFile = fopen("DataLog/moyennes", "r");
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
    TTF_Font* font = TTF_OpenFont("Font/arial_bold.ttf", 28);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *backgroundTexture = loadTexture(renderer, "Images/ScoreBoard_Background.jpg");

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

    SDL_Rect textRectPlayer, textRectBot;
    SDL_QueryTexture(texturePlayer, NULL, NULL, &textRectPlayer.w, &textRectPlayer.h);
    SDL_QueryTexture(textureBot, NULL, NULL, &textRectBot.w, &textRectBot.h);
    textRectPlayer.x = (SCREEN_WIDTH - textRectPlayer.w) / 2; // Calculate the x coordinate to center the text
    textRectBot.x = (SCREEN_WIDTH - textRectBot.w) / 2; // Calculate the x coordinate to center the text
    textRectPlayer.y = 400;
    textRectBot.y = 450;

    SDL_Rect textRectPlayerAvg, textRectBotAvg;
    SDL_QueryTexture(texturePlayerAvg, NULL, NULL, &textRectPlayerAvg.w, &textRectPlayerAvg.h);
    SDL_QueryTexture(textureBotAvg, NULL, NULL, &textRectBotAvg.w, &textRectBotAvg.h);
    textRectPlayerAvg.x = (SCREEN_WIDTH - textRectPlayerAvg.w) / 2; // Calculate the x coordinate to center the text
    textRectBotAvg.x = (SCREEN_WIDTH - textRectBotAvg.w) / 2; // Calculate the x coordinate to center the text
    textRectPlayerAvg.y = 500; // Position these lower on the screen
    textRectBotAvg.y = 550;

    // Image Scoreboard
    SDL_Texture* promptTexture = loadTexture(renderer, "Images/Scoreboard.png");
    int prompt_width, prompt_height;
    SDL_QueryTexture(promptTexture, NULL, NULL, &prompt_width, &prompt_height);
    SDL_Rect ScoreboardRect = {SCREEN_WIDTH / 2 - prompt_width / 2, 150, prompt_width, prompt_height};

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //Render the background image
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    //Render the Scoreboard image
    SDL_RenderCopy(renderer, promptTexture, NULL, &ScoreboardRect);
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

    // Frame rate
    const int FPS = 120;
    Uint32 frameDelay = 1000 / FPS;

    Uint32 frameStart;
    Uint32 frameTime;

    while (highScoreOpen)
    {
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN)
            {
                highScoreOpen = false;
            }
        }
        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    // Clean up
    SDL_DestroyTexture(texturePlayer);
    SDL_DestroyTexture(textureBot);
    SDL_DestroyTexture(texturePlayerAvg);
    SDL_DestroyTexture(textureBotAvg);
    SDL_DestroyTexture(promptTexture);
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

void cleanup0(SDL_Renderer* renderer, SDL_Window* window, Button buttons[], TTF_Font* font, Mix_Music *bgMusic, Mix_Chunk* buttonSound, SDL_Texture* logo1, SDL_Texture* logo2, SDL_Texture* bgTexture1, SDL_Texture* bgTexture2) {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    if (font) {
        TTF_CloseFont(font);
    }
    if (bgMusic) {
        Mix_FreeMusic(bgMusic);
    }
    if (buttonSound) {
        Mix_FreeChunk(buttonSound);
    }
    for(int i = 0; i < 6; ++i) {
        if (buttons[i].texture) {
            SDL_DestroyTexture(buttons[i].texture);
        }
    }
    if (logo1) {
        SDL_DestroyTexture(logo1);
    }
    if (logo2) {
        SDL_DestroyTexture(logo2);
    }
    if (bgTexture1) {
        SDL_DestroyTexture(bgTexture1);
    }
    if (bgTexture2) {
        SDL_DestroyTexture(bgTexture2);
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();
}

int main()
{
    // Initialise le générateur de nombres pseudo aléatoires
    //int seed = time(NULL);
    int seed = 42;
    srand(seed);

    if (initializeSDL() == -1)
    {
        printf("Erreur d initialisation de la SDL\n");
        return 1;
    }
    if (initializeTTF() == -1)
    {
        printf("Erreur d initialisation du font\n");
        cleanup0(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return 1;
    }
    if (initializeIMG() == -1)
    {
        printf("Erreur d initialisation des images\n");
        cleanup0(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return 1;
    }
    SDL_Window* window = createWindow("Highway Racer");
    if (window == NULL)
    {
        printf("Erreur de creation de la fenetre\n");
        cleanup0(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return 1;
    }
    SDL_Renderer* renderer = createRenderer(window);
    if (renderer == NULL)
    {
        printf("Erreur d initialisation du renderer\n");
        cleanup0(NULL, window, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        return 1;
    }
    SDL_Texture* logo1 = loadLogo(renderer, "Images/Highway-Racer.png");
    if (logo1 == NULL)
    {
        printf("Erreur du chargement du logo1\n");
        cleanup0(renderer, window, NULL, NULL, NULL, NULL, logo1, NULL, NULL, NULL);
        return 1;
    }
    SDL_Texture* logo2 = loadLogo(renderer, "Images/Menu_Image.jpg");
    if (logo2 == NULL)
    {
        printf("Erreur du chargement du logo2\n");
        cleanup0(renderer, window, NULL, NULL, NULL, NULL, logo1, logo2, NULL, NULL);
        return 1;
    }
    SDL_Texture* bgTexture1 = loadLogo(renderer, "Images/Background_Menu.png");
    SDL_Texture* bgTexture2 = loadLogo(renderer, "Images/Background_Menu.png");
    if (bgTexture1 == NULL || bgTexture2 == NULL)
    {
        printf("Erreur du chargement du background\n");
        cleanup0(renderer, window, NULL, NULL, NULL, NULL, logo1, logo2, bgTexture1, bgTexture2);
        return 1;
    }
    TTF_Font* font = loadFont("Font/arial_bold.ttf", 38);
    if (font == NULL)
    {
        printf("Erreur du chargement du font\n");
        cleanup0(renderer, window, NULL, font, NULL, NULL, logo1, logo2, bgTexture1, bgTexture2);
        return 1;
    }
    // Initialize SDL2_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL2_mixer could not initialize! SDL2_mixer Error: %s\n", Mix_GetError());
        cleanup0(renderer, window, NULL, font, NULL, NULL, logo1, logo2, bgTexture1, bgTexture2);
        return 1;
    }
    // Load the music
    Mix_Music *bgMusic = Mix_LoadMUS("Musiques/Background_Music.wav");
    if(bgMusic == NULL)
    {
        printf("Failed to load beat music! SDL2_mixer Error: %s\n", Mix_GetError());
        cleanup0(renderer, window, NULL, font, NULL, NULL, logo1, logo2, bgTexture1, bgTexture2);
        return 1;
    }
    Mix_VolumeMusic(50); // Sets the volume to half
    // Play the background music
    Mix_PlayMusic(bgMusic, -1);  // Play indefinitely

    // Load button sound
    Mix_Chunk* buttonSound = Mix_LoadWAV("Musiques/Mouse_Click.wav");
    if(buttonSound == NULL)
    {
        printf("Failed to load button sound! SDL_mixer Error: %s\n", Mix_GetError());
        cleanup0(renderer, window, NULL, font, bgMusic, NULL, logo1, logo2, bgTexture1, bgTexture2);
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
    Button buttons[6];
    for (int i = 0; i < 6; ++i)
    {
        buttons[i].rect.x = SCREEN_WIDTH / 2 - 50;
        buttons[i].rect.y = SCREEN_HEIGHT / 2 + 70 * i + 50;
        buttons[i].rect.w = 100;
        buttons[i].rect.h = 50;
        buttons[i].color.r = 0;
        buttons[i].color.g = 255;
        buttons[i].color.b = 0;
        buttons[i].color.a = 255;
        buttons[i].font = font;
    }
    buttons[0].text = "New Game";
    buttons[1].text = "Run a Bot";
    buttons[2].text = "Simulation";
    buttons[3].text = "Scoreboard";
    buttons[4].text = "Help";
    buttons[5].text = "Quit";

    for (int i = 0; i < 6; ++i)
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
    int quit = false;

    // Calcul de la nouvelle vitesse
    int bgSpeed = 1;

    // Event handler
    SDL_Event e;

    bool faireSimulation = false;
    
    int frame = 10;

    // While application is running
    while (!quit)
    {
        // Clear screen
        SDL_RenderClear(renderer);

        // Draw the background
        SDL_RenderCopy(renderer, bgTexture1, NULL, &bgRect1);
        SDL_RenderCopy(renderer, bgTexture2, NULL, &bgRect2);

	frame--;

        // Move the background
        if (frame <= 0) {
	    bgRect1.x -= bgSpeed;
	    bgRect2.x -= bgSpeed;
	    frame = 10;
	}

        // Check if the background has completely scrolled off the screen
        if (bgRect1.x + SCREEN_WIDTH < 0) bgRect1.x = SCREEN_WIDTH;
        if (bgRect2.x + SCREEN_WIDTH < 0) bgRect2.x = SCREEN_WIDTH;

        // Draw logo1 (High_Racer Text)
        SDL_Rect logo1Quad = { SCREEN_WIDTH / 2 - logo1Width / 2, 40, logo1Width, logo1Height };
        SDL_RenderCopy(renderer, logo1, NULL, &logo1Quad);

        // Define the new dimensions
        double newLogo2Width = logo2Width / 1.6;
        double newLogo2Height = logo2Height / 1.6;

        // Draw logo2 (Menu Image)
        SDL_Rect logo2Quad = { SCREEN_WIDTH / 2 - (int)newLogo2Width / 2, 20 + logo1Height, (int)newLogo2Width, (int)newLogo2Height };
        SDL_RenderCopy(renderer, logo2, NULL, &logo2Quad);

        // Draw buttons
        for (int i = 0; i < 6; ++i)
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

                for (int i = 0; i < 6; ++i)
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
                for (int i = 0; i < 6; ++i)
                {
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &(buttons[i].rect)))
                    {
                        playButtonSound(buttonSound);
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
                        if (strcmp(buttons[i].text, "Run a Bot") == 0)
                        {
                            SDL_RenderClear(renderer); // Clear the screen
                            bot robot = {0};
                            char filename[100] = "";
                            recupererBot(renderer, &quit, robot, filename);
                            LancerJeu(renderer, robot, filename+5);
                            quit = true;
                        }
                        if (strcmp(buttons[i].text, "Scoreboard") == 0)
                        {
                            SDL_RenderClear(renderer); // Clear the screen
                            displayScoreboard(renderer);
                        }
                        if (strcmp(buttons[i].text, "Simulation") == 0)
                        {

                            quit = true;
                            faireSimulation = true;

                        }
                        if (strcmp(buttons[i].text, "New Game") == 0)
                        {

                            SDL_RenderClear(renderer); // Clear the screen
                            LancerJeu(renderer, NULL, NULL); // lancer le jeu pour un humain
                            quit = true;
                        }
                    }
                }
            }
        }
    }

    cleanup0(renderer, window, buttons, font, bgMusic, buttonSound, logo1, logo2, bgTexture1, bgTexture2);

    if(faireSimulation)
    {
        lancerSimulation();
    }

    return 0;
}

