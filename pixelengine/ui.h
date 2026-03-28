#pragma once

#include <SDL3/SDL.h>

#include <stdbool.h>

#define MAX_TILESHEETS 256
#define MAX_TILES 2048

typedef int TilesheetId;

typedef int TileId;

typedef struct {
    SDL_FRect cut;
    TilesheetId tilesheetId;
} Tile;

typedef struct {
    int w;
    int h;
    SDL_Renderer *renderer;
    SDL_Texture *target;
    SDL_Texture *tilesheets[MAX_TILESHEETS];
    int tilesheetCount;
    Tile tiles[MAX_TILES];
    int tileCount;
} Ui;


void uiInit(Ui *ui, SDL_Renderer *renderer, int w, int h);

void uiDestroy(Ui *ui);

TilesheetId uiLoadTilesheet(Ui *ui, const char *path);

TileId uiCreateTile(Ui *ui, TilesheetId tilesheetId, int x, int y, int w, int h);

void uiDrawTile(Ui *ui, int x, int y, TileId tile);

void uiBeginRender(Ui *ui);

void uiFinalizeRender(Ui *ui);
