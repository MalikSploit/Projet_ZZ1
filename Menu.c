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

void displayHelp(SDL_Renderer* renderer)
{
    // Load a font
    TTF_Font* font = TTF_OpenFont("Font/arial_bold.ttf", 29);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Set the color for the text
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color

    // Set the text for the instructions
    const char* instructions[] = {
            "Instructions:",
            "",
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Copy the textures to the renderer
    SDL_Rect textRect;
    textRect.x = 50; // Adjust these values to move the text
    textRect.y = 50;
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

    // Load a font
    TTF_Font* font = TTF_OpenFont("Font/arial_bold.ttf", 24);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create the high score strings
    char highScorePlayerText[100], highScoreBotText[100];
    sprintf(highScorePlayerText, "Best player score : %s with %d points", highScores.playerName, highScores.playerScore);
    sprintf(highScoreBotText, "Best bot score : %s with %d points", highScores.botName, highScores.botScore);

    // Render the high scores
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    SDL_Surface* surfacePlayer = TTF_RenderText_Blended(font, highScorePlayerText, textColor);
    SDL_Surface* surfaceBot = TTF_RenderText_Blended(font, highScoreBotText, textColor);
    SDL_Texture* texturePlayer = SDL_CreateTextureFromSurface(renderer, surfacePlayer);
    SDL_Texture* textureBot = SDL_CreateTextureFromSurface(renderer, surfaceBot);
    SDL_FreeSurface(surfacePlayer);
    SDL_FreeSurface(surfaceBot);

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
    TTF_Font* font = loadFont("Font/arial_bold.ttf", 38);
    if (font == NULL)
    {
        return 1;
    }

    int logo1Width, logo1Height;
    SDL_QueryTexture(logo1, NULL, NULL, &logo1Width, &logo1Height);

    int logo2Width, logo2Height;
    SDL_QueryTexture(logo2, NULL, NULL, &logo2Width, &logo2Height);

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

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit)
    {
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

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
