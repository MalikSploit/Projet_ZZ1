#include "Menu.h"
#include "SDL_Initialisation.h"
#include "jeu_SDL.h"

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
            if (e.type == SDL_QUIT)
            {
                closeHelp = true;
            }
                // User presses a key
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    closeHelp = true;
                }
            }
                // User clicks the mouse
            else if (e.type == SDL_MOUSEBUTTONDOWN)
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

    PlayerScore highScore;
    highScore.score = 0;

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Parse the line
        char *name = strtok(line, ";");
        char *scoreStr = strtok(NULL, ";");

        // Remove "Player : " and "Score : " from the strings
        name += strlen("Player : ");
        scoreStr += strlen(" Score : ");

        // Convert the score to an integer
        int score = atoi(scoreStr);

        // Check if this score is higher than the current high score
        if (score > highScore.score)
        {
            strcpy(highScore.name, name);
            highScore.score = score;
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

    // Create the high score string
    char highScoreText[100];
    sprintf(highScoreText, "High score : %s with %d points", highScore.name, highScore.score);

    // Render the high score
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    SDL_Surface* surface = TTF_RenderText_Blended(font, highScoreText, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect textRect;
    SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = (SCREEN_WIDTH - textRect.w) / 2; // Calculate the x coordinate to center the text
    textRect.y = 50;

    // Clear the renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Copy the texture with the high score to the renderer
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Show the renderer contents on the screen
    SDL_RenderPresent(renderer);

    // Wait until user presses a key
    bool highScoreOpen = true;
    SDL_Event e;
    while (highScoreOpen)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                highScoreOpen = false;
            }
        }
    }

    // Clean up
    SDL_DestroyTexture(texture);
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
    buttons[2].text = "Highscore";
    buttons[3].text = "Help";
    buttons[4].text = "Quit";

    for (int i = 0; i < 5; ++i)
    {
        SDL_Surface* surface = TTF_RenderText_Blended(font, buttons[i].text, buttons[i].color);
        buttons[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

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
            // User presses a key
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_q:
                        quit = true;
                        break;
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
                        printf("%s button clicked!\n", buttons[i].text);
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
                        if (strcmp(buttons[i].text, "Highscore") == 0)
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
