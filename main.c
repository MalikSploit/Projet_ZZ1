#include "main.h"
#include "SDL_Initialisation.h"

UserCar initVoiture(SDL_Renderer *renderer)
{
    UserCar userCar;

    // Set up initial size and position of the user car
    userCar.cell_x = 4;
    userCar.cell_y = 7;
    userCar.rect.w = 140;
    userCar.rect.h = 140;
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

EnemyCar initObstacle(SDL_Renderer *renderer, int lane_x, int lane_y, char *obstacleImagePath)
{
    EnemyCar enemyCar;

    // Set up initial size and position of the enemy car
    enemyCar.cell_x = lane_x;
    enemyCar.cell_y = lane_y;
    enemyCar.rect.w = 140;
    enemyCar.rect.h = 140;
    enemyCar.last_update = SDL_GetTicks();

    enemyCar.speed = rand() % 4;

    // Load the texture for the enemy car
    SDL_Surface *tempSurface = LoadImage(obstacleImagePath);

    enemyCar.texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return enemyCar;
}

bool checkCollision(int grid[8][8], int x, int y)
{
    return grid[x][y] == 1;
}

EnemyCar initRandomObstacle(SDL_Renderer *renderer, int grid[8][8])
{
    // Generate a random lane (between 0 and 7)
    int x = rand() % 8;
    int y = rand() % 4;

    // Check if the cell is already occupied
    while(checkCollision(grid, x, y))
    {
        x = rand() % 8;
        y = rand() % 4;
    }

    // Mark the cell as occupied
    grid[x][y] = 1;

    // Generate a random obstacle number (between 1 and 8)
    int obstacleNumber = rand() % 7 + 1;

    // Construct the file path for the obstacle image
    char obstacleImagePath[25];
    sprintf(obstacleImagePath, "Images/Obstacle%d.png", obstacleNumber);

    // Initialize the obstacle
    return initObstacle(renderer, x, y, obstacleImagePath);
}

void drawObstacle(SDL_Renderer *renderer, EnemyCar *enemyCar)
{
    enemyCar->rect.x = enemyCar->cell_x * (TAILLE_CELLULE_LARGEUR/2) + 250;
    enemyCar->rect.y = enemyCar->cell_y * (TAILLE_CELLULE_LONGUEUR/2); // start at the bottom of the screen

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


void moveObstacles(SDL_Renderer *renderer, int grid[8][8], EnemyCar *obstacles, int *numObstacles, UserCar *userCar, bool *running)
{
    for(int i = 0; i < *numObstacles; i++)
    {
        Uint32 current_time = SDL_GetTicks();
        if (current_time - obstacles[i].last_update >= (Uint32)(5000 / (obstacles[i].speed + userCar->velocity)))
        {
            // Enough time has passed, update the position and remember the current time
            obstacles[i].cell_y++;
            obstacles[i].last_update = current_time;
        }

        // If the obstacle has moved off the bottom of the screen,
        // move it back to the top and select a random lane.
        if (obstacles[i].cell_y >= 8)
        {
            // Free the previous texture before creating a new one
            SDL_DestroyTexture(obstacles[i].texture);

            // Initialize a new obstacle
            obstacles[i] = initRandomObstacle(renderer, grid);
        }
        // Check for collision with the user car
        if (SDL_HasIntersection(&userCar->rect, &obstacles[i].rect))
        {
            *running = false; // end the game
            break;
        }
    }
}

void cleanup(SDL_Surface* backgroundSurface, SDL_Texture* backgroundTexture, SDL_Texture* backgroundTexture2,
             SDL_Texture* scoreTexture, SDL_Texture* pauseTexture, SDL_Texture* vitesseTexture, TTF_Font* font,
             UserCar userCar, EnemyCar obstacles[], SDL_Renderer* renderer, SDL_Window* window)
{
    // Free the loaded surface as it is no longer needed
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(backgroundTexture2);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(pauseTexture);
    SDL_DestroyTexture(vitesseTexture);
    TTF_CloseFont(font);

    // Destroy user car textures
    for(int i = 0; i < userCar.total_frames; ++i)
    {
        SDL_DestroyTexture(userCar.textures[i]);
    }

    // Destroy enemy car textures
    for(int i = 0; i < MAX_OBSTACLES; i++)
    {
        SDL_DestroyTexture(obstacles[i].texture);
    }

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}

void InitScore(SDL_Renderer* renderer, Uint32* lastScoreUpdateTime, int* score, TTF_Font* font, SDL_Texture** scoreTexture, SDL_Rect *scoreRect, UserCar* userCar, SDL_Color* textColor)
{
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - *lastScoreUpdateTime >= 1000)
    {
        // if 1 second has passed
        *score = *score + userCar->velocity;
        *lastScoreUpdateTime = currentTime; // update the time of the last score update
    }

    // update the score text
    char scoreText[20];
    sprintf(scoreText, "Score: %d", *score);
    updateText(renderer, font, *textColor, scoreTexture, scoreRect, scoreText);
}

void initVitesse(UserCar* userCar, SDL_Renderer *renderer, SDL_Color *textColor, SDL_Texture** vitesseTexture, SDL_Rect *vitesseRect, TTF_Font* font)
{

    char vitesseText[20];
    sprintf(vitesseText, "Speed: %d km/h", userCar->velocity * 10);
    updateText(renderer, font, *textColor, vitesseTexture, vitesseRect, vitesseText);
}


int LancerJeu()
{
    // Initialize SDL
    initializeSDL();

    // Initialize SDL_image
    initializeIMG();

    // Create window
    SDL_Window *window = createWindow("Highway Racer");

    // Create renderer
    SDL_Renderer *renderer = createRenderer(window);

    // Load the background image
    SDL_Surface *backgroundSurface = LoadImage("Images/Game.png");

    // Create a texture from the loaded surface
    SDL_Texture *backgroundTexture = LoadTexture(renderer, backgroundSurface);

    // Create a second texture from the same surface for scrolling background
    SDL_Texture *backgroundTexture2 = LoadTexture(renderer, backgroundSurface);

    initializeTTF();
    TTF_Font* font = loadFont("Font/arial_bold.ttf", 28);

    SDL_Color textColor = {255, 255, 255, 255}; // white color
    SDL_Texture* scoreTexture = NULL;
    SDL_Texture* vitesseTexture = NULL;
    SDL_Rect scoreRect = {10, 10, 0, 0}; // position for score, top left
    SDL_Rect vitesseRect = {SCREEN_WIDTH - 260, 10, 0, 0}; // position for speed, top right

    UserCar userCar = initVoiture(renderer);
    int numObstacles = MAX_OBSTACLES; // Keep track of the current number of obstacles
    EnemyCar obstacles[MAX_OBSTACLES];

    // Initialize a grid to keep track of which cells are occupied
    int grid[8][8] = { { 0 } };

    // Initialize all the obstacles
    for(int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacles[i] = initRandomObstacle(renderer, grid);
    }

    int bgScroll = 0;  // Initialize background scroll offset

    int score = 0;
    Uint32 lastScoreUpdateTime = SDL_GetTicks();

    bool isPaused = false;
    SDL_Texture* pauseTexture = NULL;
    SDL_Rect pauseRect = {SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 50, 100, 100}; // position for pause text, middle of the screen

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
                        if (userCar.cell_x > 0)
                        {
                            userCar.cell_x -= 1; // Move the car to the left
                            break;
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_RIGHT)
                    {
                        if (userCar.cell_x < 7)
                        {
                            userCar.cell_x += 1; // Move the car to the right
                            break;
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_UP && !( SDL_GetModState() & KMOD_CTRL ))
                    {
                        if (userCar.cell_y > 0) // Add some condition here to prevent the car from moving off the screen
                        {
                            userCar.cell_y -= 1; // Move the car forward (upwards on the screen)
                        }
                    }
                    else if (e.key.keysym.sym == SDLK_DOWN && !( SDL_GetModState() & KMOD_CTRL ))
                    {
                        if (userCar.cell_y < 7)
                        {
                            userCar.cell_y += 1;
                            break;
                        }
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

        if (!isPaused)
        {

            InitScore(renderer, &lastScoreUpdateTime, &score, font, &scoreTexture, &scoreRect, &userCar, &textColor);

            initVitesse(&userCar, renderer, &textColor, &vitesseTexture, &vitesseRect, font);


             // Handle obstacle movements and user car collision
            moveObstacles(renderer, grid, obstacles, &numObstacles, &userCar, &running);

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

        drawVoiture(renderer, &userCar);

        // Draw the obstacles
        for(int i = 0; i < MAX_OBSTACLES; i++)
        {
            drawObstacle(renderer, &obstacles[i]);
        }

        // Draw the score and time
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_RenderCopy(renderer, vitesseTexture, NULL, &vitesseRect);

        // If game is paused, draw the pause message
        if (isPaused)
        {
            SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);
        }

        // Update screen
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
    }

    cleanup(backgroundSurface, backgroundTexture, backgroundTexture2, scoreTexture, pauseTexture, vitesseTexture, font, userCar, obstacles, renderer, window);

    return 1;
}