#pragma once

#include "ui.h"

typedef struct {
    void *userData;
    SDL_Renderer *renderer;
    SDL_Window *window;    
    Ui ui;
} PixelEngine;

typedef void (*PeGameUpdateFunc)(void *userData);

typedef void (*PeGameRenderFunc)(void *userData, Ui *ui);

bool peInit(PixelEngine *pe, int width, int height, char *title);

void peRun(PixelEngine *pe, PeGameUpdateFunc gameUpdateFunc, PeGameRenderFunc gameRenderFunc, void *userData);

void peDestroy(PixelEngine *pe);