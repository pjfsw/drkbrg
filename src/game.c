#include "game.h"

static const int horisontalTiles = 13;
static const int verticalTiles = 10;
static const int tileSize = 32;
static const int squareSize = tileSize+1;
static const int maxTurns = 26;
static const uint32_t gridColor = 0x282422ff;
static const uint32_t lifeColor = 0xcc1111ff;
static const uint32_t textColor = 0xffffffff;

static void updateSundialString(Game *game) {
    memset(game->sundialString, 0, sizeof(game->sundialString));
    SDL_itoa(game->sundial, game->sundialString, 10);
}

static int createHeroTile(Graphics *g, Ui *ui, Hero *hero) {
    char buf[100];
    memset(buf, 0, sizeof(buf));
    strcpy(buf, hero->name);
    return uiCreateTextboxTile(ui, buf, 104,128, 0xffffffff, 0xffffffff, 0x383432ff, true);
}

static void initUiElements(Game *game, Ui *ui) {
    Graphics *g = &game->g;
    for (int i = 0; i < 4; i++) {
        g->hero[i] = createHeroTile(g, ui, &game->hero[i]);
    }
    int bx = (uiWidth(ui) - (horisontalTiles * squareSize)) / 2;
    int bw = horisontalTiles * tileSize + 1;
    int by = (uiHeight(ui) - (verticalTiles * squareSize)) / 2;
    int bh = verticalTiles * tileSize + 1;
    uiContextSet(ui, &g->boardCtx, bx, by, bw, bh);
    uiContextSet(ui, &g->rPanelCtx, bx + bw + 8, by, uiWidth(ui), uiHeight(ui));
}

static void nextRound(Game *game) {
    updateSundialString(game);
}

void gameInit(void *userData, Ui *ui) {
    Game *game = (Game*)userData;
    memset(game, 0, sizeof(Game));
    for (int i = 0; i < 4; i++) {
        game->hero[i].npc = true;
        game->hero[i].life = i + 7;
    }       
    strcpy(game->hero[0].name, "RIDDAR ROHAN");
    strcpy(game->hero[1].name, "SIGEIR SHARPYXE");
    strcpy(game->hero[2].name, "RIDDAR RUT");
    strcpy(game->hero[3].name, "BARDHOR BAGMAN");

    game->sundial = 1;
    if (game->hero[0].npc) {
        game->gameplayState = GPS_NEXT_NPC;
    } else {
        game->gameplayState = GPS_WAIT_PLAYER;
    }
    game->gameState = GS_PLAY;
    nextRound(game);
    initUiElements(game, ui);
}

static void gpsNextNpc(Game *game) {
    game->t = 0;
    game->gameplayState = GPS_WAIT_NPC;
}

static void gameOver(Game *game) {
    game->gameState = GS_GAMEOVER;
}

static void nextPlayer(Game *game) {
    game->currentHero++;
    if (game->currentHero > 3) {
        game->currentHero = 0;
        game->sundial++;
    }
    if (game->sundial > maxTurns) {
        gameOver(game);
    } else {
        nextRound(game);
        if (game->hero[game->currentHero].npc) {
            game->gameplayState = GPS_NEXT_NPC;
        } else {
            game->gameplayState = GPS_WAIT_PLAYER;
        }
    }
}

static void gpsWaitNpc(Game *game, double deltaTime) {
    game->t += deltaTime;
    if (game->t > 0.5) {
        nextPlayer(game);
    }
}

static void gpsWaitPlayer(Game *game) {
}

static void gamePlay(Game *game, double deltaTime) {
    switch (game->gameplayState) {
        case GPS_NEXT_NPC:
            gpsNextNpc(game);
            break;
        case GPS_WAIT_NPC:
            gpsWaitNpc(game, deltaTime);
            break;
        case GPS_WAIT_PLAYER:
            gpsWaitPlayer(game);
            break;
    }
}

void gameUpdate(void *userData, double deltaTime) {
    Game *game = (Game *)userData;
    switch (game->gameState) {
        case GS_INIT:
            break;
        case GS_PLAY:
            gamePlay(game, deltaTime);
            break;
        case GS_GAMEOVER:
            break;
    }
}

static int getScreenX(const Ui *ui, int x) {
    return x * tileSize;
}

static int getScreenY(const Ui *ui, int y) {
    return y * tileSize;
}

static void drawBoard(Graphics *g, const Ui *ui) {
    UiContext *ctx = &g->boardCtx;

    for (int x = 0; x < horisontalTiles; x++) {
        for (int y = 0; y < verticalTiles; y++) {
            uiDrawRect(ctx, getScreenX(ui, x), getScreenY(ui, y), squareSize, squareSize, gridColor);
        }
    }
    uiFillRect(ctx, getScreenX(ui, 0), getScreenY(ui, 0), squareSize, squareSize, gridColor);
    uiFillRect(ctx, getScreenX(ui, horisontalTiles-1), getScreenY(ui, 0), squareSize, squareSize, gridColor);
    uiFillRect(ctx, getScreenX(ui, 0), getScreenY(ui, verticalTiles-1), squareSize, squareSize, gridColor);
    uiFillRect(ctx, getScreenX(ui, horisontalTiles-1), getScreenY(ui, verticalTiles-1), squareSize, squareSize, gridColor);
}


static void drawRightPanel(Game *game, const Ui *ui) {
    int offset = (uiWidth(ui) - (horisontalTiles * squareSize)) / 2 + horisontalTiles * tileSize + 8;
    
    UiContext *ctx = &game->g.rPanelCtx;
    
    int y = 0;
    uiWrite(ctx, "TURN   /26", 0, y, textColor);
    uiWrite(ctx, game->sundialString, 40, y, textColor);    
    y+=16;

    uiDrawTile(ctx, 0, y, game->g.hero[game->currentHero]);

    Hero *hero = &game->hero[game->currentHero];

    y+=128;

    const int lifeSize = 8;
    for (int i = 0; i < hero->life; i++) {
        uiFillRect(ctx, i * lifeSize, y, lifeSize-1, lifeSize, lifeColor); 
    }


}
void gameRender(void *userData, const Ui *ui) {
    Game *game = (Game*)userData;
    drawBoard(&game->g, ui);
    drawRightPanel(game, ui);
    if (game->gameState == GS_GAMEOVER) {
        UiContext ctx;
        uiContextSetDefault(ui, &ctx);
        uiWrite(&ctx, "GAME OVER!", (uiWidth(ui)-80)/2, (uiHeight(ui)-FONT_HEIGHT)/2, 0xff3333ff);
    }
}
