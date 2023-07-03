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
