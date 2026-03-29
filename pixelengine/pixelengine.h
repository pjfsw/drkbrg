#pragma once

#include "ui.h"

typedef struct {
    void *userData;
    SDL_Renderer *renderer;
    SDL_Window *window;    
    Ui ui;
} PixelEngine;

typedef void (*PeGameInitFunc)(void *userData, Ui *ui);

typedef void (*PeGameUpdateFunc)(void *userData, double deltaTime);

typedef void (*PeGameRenderFunc)(void *userData, const Ui *ui);

bool peInit(PixelEngine *pe, int width, int height, char *title);

void peRun(PixelEngine *pe, PeGameInitFunc gameInitFunc, PeGameUpdateFunc gameUpdateFunc,
    PeGameRenderFunc gameRenderFunc, void *userData);

void peDestroy(PixelEngine *pe);