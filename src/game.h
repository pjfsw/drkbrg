#pragma once

#include "ui.h"
#include "hero.h"

typedef enum {
    GS_INIT,
    GS_PLAY,
} GameState;

typedef enum {
    GPS_WAIT_NPC,
    GPS_WAIT_PLAYER
} GamePlayState;

typedef struct {
    TileId foo;
} Tiles;

typedef struct {
    GameState gameState;
    Hero hero[4];
    int currentHero;
    Tiles tiles;
} Game;

void gameInit(void *userData, Ui *ui);

void gameUpdate(void *userData, double deltaTime);

void gameRender(void *userData, const Ui *ui);
