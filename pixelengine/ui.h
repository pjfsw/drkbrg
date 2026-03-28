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

typedef struct {
    const Ui *ui;
    SDL_Rect cut;
    bool useCut;
} UiContext;

void uiInit(Ui *ui, SDL_Renderer *renderer, int w, int h);

void uiDestroy(Ui *ui);

int uiWidth(const Ui *ui);

int uiHeight(const Ui *ui);

TilesheetId uiLoadTilesheet(Ui *ui, const char *path);

TileId uiCreateTile(Ui *ui, TilesheetId tilesheetId, int x, int y, int w, int h);

const UiContext *uiContextGetDefault(const Ui *ui);

void uiContextSet(const Ui *ui, UiContext *ctx, int x, int y, int w, int h);

void uiDrawTile(const UiContext *ctx, int x, int y, TileId tile);

void uiDrawRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color, bool fill);

void uiFillRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color);

void uiBeginRender(Ui *ui);

void uiFinalizeRender(Ui *ui);
