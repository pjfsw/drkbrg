#pragma once

#include "ui.h"
#include "hero.h"
#include "room.h"

#define NUMBER_OF_HEROES 4
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
    int x;
    int y;
} Coord;

typedef struct {
    TileId hero[NUMBER_OF_HEROES];
    UiContext boardCtx;
    UiContext rPanelCtx;
    uint16_t frame;
} Graphics;

typedef struct {
    GameState gameState;
    GameplayState gameplayState;
    Hero hero[NUMBER_OF_HEROES];
    Coord heroPos[NUMBER_OF_HEROES];
    Coord entrancePositions[4];
    int currentHero;
    int sundial;
    char sundialString[8];
    double t;
    Graphics g;
} Game;

void gameInit(void *userData, Ui *ui);

void gameUpdate(void *userData, double deltaTime);

void gameRender(void *userData, const Ui *ui);
