#pragma once

#include "ui.h"
#include "hero.h"

typedef enum {
    GS_INIT,
    GS_PLAY,
    GS_GAMEOVER
} GameState;

typedef enum {
    GPS_NEXT_NPC,
    GPS_WAIT_NPC,
    GPS_WAIT_PLAYER
} GameplayState;

typedef struct {
    TileId hero[4];
    UiContext boardCtx;
    UiContext rPanelCtx;
} Graphics;

typedef struct {
    GameState gameState;
    GameplayState gameplayState;
    Hero hero[4];
    int currentHero;
    int sundial;
    char sundialString[8];
    double t;
    Graphics g;
} Game;

void gameInit(void *userData, Ui *ui);

void gameUpdate(void *userData, double deltaTime);

void gameRender(void *userData, const Ui *ui);
