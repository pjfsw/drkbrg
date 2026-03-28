#pragma once

#include "ui.h"

typedef enum {
    GS_INIT,
    GS_PLAY,
} GameState;

typedef struct {
    GameState gameState;
} Game;

void gameInit(Game *game);

void gameUpdate(void *userData);

void gameRender(void *userData, Ui *ui);
