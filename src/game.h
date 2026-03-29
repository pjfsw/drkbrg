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
    UiContext boardCtx;
    UiContext rPanelCtx;
} Graphics;

typedef struct {
    GameState gameState;
    Hero hero[4];
    int currentHero;
    int sundial;
    char sundialString[8];
    Graphics g;
} Game;

void gameInit(void *userData, Ui *ui);

void gameUpdate(void *userData, double deltaTime);

void gameRender(void *userData, const Ui *ui);
