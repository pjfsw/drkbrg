#include <stdbool.h>
#include <stdio.h>

#include "game.h"
#include "ui.h"

void run() {
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }
    /* Calculate 75% of desktop size */
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(displayID);
    int windowW = (int)(mode->w * 0.75f);
    int windowH = (int)(mode->h * 0.75f);

    SDL_Window *window = SDL_CreateWindow("Drkbrg", windowW, windowH, SDL_WINDOW_RESIZABLE);

    if (window == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    bool fullScreen = false;
    SDL_Event event;
    
    Ui ui;
    uiInit(&ui, renderer);
    Game game;
    gameInit(&game);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                /* Check for ALT + ENTER */
                if (event.key.key == SDLK_RETURN && (event.key.mod & SDL_KMOD_ALT)) {
                    if (fullScreen == true) {
                        SDL_SetWindowFullscreen(window, false);
                        fullScreen = false;
                    } else {
                        SDL_SetWindowFullscreen(window, true);
                        fullScreen = true;
                    }
                }
            }
        }

        /* Rendering */
        SDL_SetRenderDrawColor(renderer, 26, 26, 26, 255); /* Dark background */
        SDL_RenderClear(renderer);

        uiBeginRender(&ui);
        gameUpdate(&game);
        gameRender(&game, &ui);
        uiFinalizeRender(&ui);        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}