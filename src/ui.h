#pragma once

#include <SDL3/SDL.h>

#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 360
#define MAX_H_TILES 32
#define MAX_V_TILES 32
#define TILE_SIZE 64

typedef struct {
    SDL_Renderer *renderer;
    SDL_Texture *target;
} Ui;

void uiInit(Ui *ui, SDL_Renderer *renderer);

void uiDestroy(Ui *ui);

void uiDrawTile(Ui *ui, int x, int y, int tile);

void uiBeginRender(Ui *ui);
void uiFinalizeRender(Ui *ui);
