#include "Menu.h"
#include "Constantes.h"


void displayHelp()
{
    // Create a new window for the help screen
    SDL_Window* helpWindow = SDL_CreateWindow("Help", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if (helpWindow == NULL)
    {
        printf("Help window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create a renderer for the help window
    SDL_Renderer* helpRenderer = SDL_CreateRenderer(helpWindow, -1, SDL_RENDERER_ACCELERATED);
    if (helpRenderer == NULL)
    {
        printf("Help renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(helpWindow);
        return;
    }

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
        textures[i] = SDL_CreateTextureFromSurface(helpRenderer, surface);
        SDL_FreeSurface(surface);
    }

    // Clear the renderer
    SDL_SetRenderDrawColor(helpRenderer, 0, 0, 0, 255);
    SDL_RenderClear(helpRenderer);

    // Copy the textures to the renderer
    SDL_Rect textRect;
    textRect.x = 50; // Adjust these values to move the text
    textRect.y = 50;
    for (int i = 0; i < numLines; i++)
    {
        SDL_QueryTexture(textures[i], NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(helpRenderer, textures[i], NULL, &textRect);
        textRect.y += textRect.h + 5; // Move down for the next line (adjust the value to increase or decrease the line spacing)
    }

    // Show the renderer contents on the screen
    SDL_RenderPresent(helpRenderer);

    // Wait until user closes the help window
    bool helpOpen = true;
    SDL_Event e;
    while (helpOpen)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if ((e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(helpWindow)) || ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)))
            {
                helpOpen = false;
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
    SDL_DestroyRenderer(helpRenderer);
    SDL_DestroyWindow(helpWindow);
}


void displayHighScore()
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

    // Create a new window for the high score
    SDL_Window* highScoreWindow = SDL_CreateWindow("High Score", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (highScoreWindow == NULL)
    {
        printf("High Score window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    // Create a renderer for the high score window
    SDL_Renderer* highScoreRenderer = SDL_CreateRenderer(highScoreWindow, -1, SDL_RENDERER_ACCELERATED);
    if (highScoreRenderer == NULL) {
        printf("High Score renderer could not be created! SDL Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(highScoreWindow);
        return;
    }

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
    SDL_Texture* texture = SDL_CreateTextureFromSurface(highScoreRenderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect textRect;
    SDL_QueryTexture(texture, NULL, NULL, &textRect.w, &textRect.h);
    textRect.x = (640 - textRect.w) / 2; // Calculate the x coordinate to center the text
    textRect.y = 50;

    // Clear the renderer
    SDL_SetRenderDrawColor(highScoreRenderer, 0, 0, 0, 255);
    SDL_RenderClear(highScoreRenderer);

    // Copy the texture with the high score to the renderer
    SDL_RenderCopy(highScoreRenderer, texture, NULL, &textRect);

    // Show the renderer contents on the screen
    SDL_RenderPresent(highScoreRenderer);

    // Wait until user closes the high score window
    bool highScoreOpen = true;
    SDL_Event e;
    while (highScoreOpen)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if ((e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(highScoreWindow)) || ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)))
            {
                highScoreOpen = false;
            }
        }
    }

    // Clean up
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(highScoreRenderer);
    SDL_DestroyWindow(highScoreWindow);
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

void drawText(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color color, int x, int y)
{

    // Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    // Create texture from surface pixels
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL) {
        printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        return;
    }

    // Set text dimensions
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };

    // Render text
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    // Free surface and texture
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


SDL_Texture* loadTexture(SDL_Renderer* renderer, char* filepath)
{
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(filepath);
    if(loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", filepath, IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", filepath, SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}


