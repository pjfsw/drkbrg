#include "pixelengine.h"

#include <stdbool.h>
#include <stdio.h>

#include "ui.h"

bool peInit(PixelEngine *pe, int width, int height, char *title) {
    memset(pe, 0, sizeof(PixelEngine));
    if (SDL_Init(SDL_INIT_VIDEO) == false) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return false;
    }

    /* Calculate 75% of desktop size */
    SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(displayID);
    int windowW = (int)(mode->w * 0.75f);
    int windowH = (int)(mode->h * 0.75f);

    pe->window = SDL_CreateWindow(title, windowW, windowH, SDL_WINDOW_RESIZABLE);

    if (pe->window == NULL) {
        SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    pe->renderer = SDL_CreateRenderer(pe->window, NULL);
    if (pe->renderer == NULL) {
        SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_DestroyWindow(pe->window);
        SDL_Quit();
        return false;
    }
    uiInit(&pe->ui, pe->renderer, width, height);
    return true;

}

void peRun(PixelEngine *pe, PeGameInitFunc gameInitFunc, PeGameUpdateFunc gameUpdateFunc, PeGameRenderFunc gameRenderFunc, void *userData) {
    bool quit = false;
    bool fullScreen = false;
    SDL_Event event;
    
    gameInitFunc(userData, &pe->ui);
    SDL_SetRenderTarget(pe->renderer, NULL);

    uint64_t lastTick = SDL_GetTicksNS();
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            } else if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.mod & SDL_KMOD_ALT) {
                    if (event.key.key == SDLK_RETURN) {
                        if (fullScreen == true) {
                            SDL_SetWindowFullscreen(pe->window, false);
                            fullScreen = false;
                        } else {
                            SDL_SetWindowFullscreen(pe->window, true);
                            fullScreen = true;
                        }
                    }
                    if (event.key.key == SDLK_Q) {
                        quit = true;
                    }
                }
            }
        }

        /* Rendering */
        SDL_SetRenderDrawColor(pe->renderer, 0, 0, 0, 255); /* Dark background */
        SDL_RenderClear(pe->renderer);

        uiBeginRender(&pe->ui);
        uint64_t currentTick = SDL_GetTicksNS();
        double deltaTime = (double)(currentTick - lastTick) / 1000000000.0;
        gameUpdateFunc(userData, deltaTime);
        gameRenderFunc(userData, &pe->ui);
        uiFinalizeRender(&pe->ui);        
        SDL_RenderPresent(pe->renderer);
    }
}

void peDestroy(PixelEngine *pe) {
    uiDestroy(&pe->ui);
    if (pe->renderer != NULL) {
        SDL_DestroyRenderer(pe->renderer);
    }
    if (pe->window != NULL) {        
        SDL_DestroyWindow(pe->window);
    }
    SDL_Quit();
}