#pragma once

#include "ui.h"
#include "hero.h"

typedef enum {
    GS_INIT,
    GS_PLAY,
} GameState;

typedef struct {
    GameState gameState;
    Hero hero[4];
    int currentHero;
} Game;

void gameInit(Game *game);

void gameUpdate(void *userData);

void gameRender(void *userData, const Ui *ui);
