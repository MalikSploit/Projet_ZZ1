#include "Constantes.h"
#include "main.h"

typedef struct UserCar {
    SDL_Rect rect;
    SDL_Texture *texture;
    int velocity;
} UserCar;

UserCar initVoiture(SDL_Renderer *renderer)
{
    UserCar userCar;

    // Set up initial size and position of the user car
    userCar.rect.w = 100;
    userCar.rect.h = 100;
    userCar.rect.x = SCREEN_WIDTH / 2 + 35;
    userCar.rect.y = SCREEN_HEIGHT - 100 - userCar.rect.h;// start at the bottom of the screen

    // Load the texture for the user car
    SDL_Surface *tempSurface = IMG_Load("Images/Car.png");
    if (!tempSurface) {
        printf("Unable to load image car.png! SDL_image Error: %s\n", IMG_GetError());
        // handle error here
    }

    userCar.texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    // Initial velocity
    userCar.velocity = 5;

    return userCar;
}

void drawVoiture(SDL_Renderer *renderer, UserCar *userCar) {
    SDL_RenderCopy(renderer, userCar->texture, NULL, &userCar->rect);
}

int LancerJeu()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized. SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not be initialized. SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("Highway Racer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created. SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    // Load the background image
    SDL_Surface *backgroundSurface = IMG_Load("Images/Game.png");
    if (backgroundSurface == NULL)
    {
        printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return 1; // Return error
    }

    // Create a texture from the loaded surface
    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture == NULL)
    {
        printf("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
        return 1; // Return error
    }

    // Create a second texture from the same surface for scrolling background
    SDL_Texture *backgroundTexture2 = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture2 == NULL)
    {
        printf("Unable to create texture2 from surface! SDL Error: %s\n", SDL_GetError());
        return 1; // Return error
    }

    UserCar userCar = initVoiture(renderer);

    int bgScroll = 0;  // Initialize background scroll offset

    bool running = true;
    SDL_Event e;
    while (running)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if ((e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_CLOSE && e.window.windowID == SDL_GetWindowID(window)) || ((e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)))
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_LEFT)
                {
                    userCar.rect.x -= 180; // Move the car to the left
                    break;
                } else if (e.key.keysym.sym == SDLK_RIGHT)
                {
                    userCar.rect.x += 180; // Move the car to the right
                    break;
                }
                if (e.key.keysym.sym == SDLK_UP)
                {
                    userCar.velocity += 5;
                    break;
                }
                if (e.key.keysym.sym == SDLK_DOWN)
                {
                    if (userCar.velocity > 5)
                    {
                        userCar.velocity -= 5;
                        break;
                    }
                }
            }
        }
        bgScroll += userCar.velocity;  // Scroll background at car's velocity

        // Reset scroll offset when it is greater than or equal to the height of the screen
        if (bgScroll >= SCREEN_HEIGHT)
        {
            bgScroll = 0;
        }

        // Draw the scrolling backgrounds
        SDL_Rect bgQuad1 = { 0, bgScroll - SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_Rect bgQuad2 = { 0, bgScroll, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &bgQuad1);
        SDL_RenderCopy(renderer, backgroundTexture2, NULL, &bgQuad2);

        drawVoiture(renderer, &userCar);
        // Update screen
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    // Free the loaded surface as it is no longer needed
    SDL_FreeSurface(backgroundSurface);

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();

    return 1;
}