#include "sdl_utils.h"

int init_SDL(SDL_Window **window, SDL_Renderer **renderer, const char *title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!*window) {
        fprintf(stderr, "Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        fprintf(stderr, "Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

void event_loop(SDL_Renderer *renderer, int x_offset, int y_offset, int square_size) {
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                SDL_Rect square = {x_offset + col * square_size, y_offset + row * square_size, square_size, square_size};
                SDL_SetRenderDrawColor(renderer, (row + col) % 2 == 0 ? 255 : 0, (row + col) % 2 == 0 ? 255 : 0, (row + col) % 2 == 0 ? 255 : 0, 255);
                SDL_RenderFillRect(renderer, &square);
            }
        }
        
        SDL_RenderPresent(renderer);
    }
}

void cleanup_SDL(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
