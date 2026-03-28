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

void gameUpdate(Game *game);

void gameRender(Game *game, Ui *ui);
