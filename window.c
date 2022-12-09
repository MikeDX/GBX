#include <SDL2/SDL.h>

// Window dimensions
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// The SDL window
SDL_Window *window = NULL;

// The SDL renderer
SDL_Renderer *renderer = NULL;

int create_window()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error: Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Create the SDL window
    window = SDL_CreateWindow("GBX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Error: Failed to create SDL window: %s\n", SDL_GetError());
        return 1;
    }

    // Create the SDL renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Error: Failed to create SDL renderer: %s\n", SDL_GetError());
        return 1;
    }

    return 0;
}

void destroy_window()
{
    // Destroy the SDL renderer
    if (renderer)
        SDL_DestroyRenderer(renderer);

    // Destroy the SDL window
    if (window)
        SDL_DestroyWindow(window);

    // Quit SDL
    SDL_Quit();
}
