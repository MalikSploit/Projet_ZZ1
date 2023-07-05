#include "SDL_Initialisation.h"
#include "Constantes.h"
#include "Menu.h"

SDL_Window* createWindow(char *title)
{
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }
    return window;
}

SDL_Renderer* createRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return NULL;
    }
    return renderer;
}
int initializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    return 0;
}

int initializeTTF()
{
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return -1;
    }
    return 0;
}

int initializeIMG()
{
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    return 0;
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

SDL_Texture* loadLogo(SDL_Renderer* renderer, char *imagePath)
{
    SDL_Texture* logo = loadTexture(renderer, imagePath);
    if (logo == NULL)
    {
        printf("Failed to load logo image!\n");
        return NULL;
    }
    return logo;
}

TTF_Font* loadFont(char* fontPath, int size)
{
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    if (font == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }
    return font;
}

SDL_Surface* LoadImage(char *imagePath)
{
    SDL_Surface *backgroundSurface = IMG_Load(imagePath);
    if (backgroundSurface == NULL)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return NULL; // Return error
    }
    return backgroundSurface;
}

SDL_Texture *LoadTexture(SDL_Renderer *renderer, SDL_Surface *backgroundSurface)
{
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture == NULL)
    {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return NULL; // Return error
    }
    return backgroundTexture;
}

SDL_Texture* createTextTexture(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const char* text)
{
    // Create a surface from the text
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (textSurface == NULL)
    {
        printf("Failed to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }

    // Create a texture from the surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == NULL)
    {
        printf("Failed to create text texture! SDL Error: %s\n", SDL_GetError());
    }

    // Get rid of the old surface
    SDL_FreeSurface(textSurface);

    return textTexture;
}
