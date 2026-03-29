#include "game.h"

static const int horisontalTiles = 13;
static const int verticalTiles = 10;
static const int tileSize = 32;
static const int squareSize = tileSize+1;
static const uint32_t gridColor = 0x282422ff;
static const uint32_t lifeColor = 0xcc1111ff;

void gameInit(void *userData, Ui *ui) {
    Game *game = (Game*)userData;
    memset(game, 0, sizeof(Game));
    for (int i = 0; i < 4; i++) {
        game->hero[i].npc = true;
        game->hero[i].life = i + 7;
    }       
    game->tiles.foo = uiCreateTextboxTile(ui, "FOO", 64,64, 0xffffffff, 0x0000ffff, true);
}

void gameUpdate(void *userData, double deltaTime) {
    Game *game = (Game*)userData;
}

static int getScreenX(const Ui *ui, int x) {
    return x * tileSize;
}

static int getScreenY(const Ui *ui, int y) {
    return y * tileSize;
}

static void drawBoard(const Ui *ui) {
    UiContext ctx;
    uiContextSet(ui,
        &ctx,
        (uiWidth(ui) - (horisontalTiles * squareSize)) / 2,
        (uiHeight(ui) - (verticalTiles * squareSize)) / 2,
        horisontalTiles * tileSize + 1,
        verticalTiles * tileSize + 1);

    for (int x = 0; x < horisontalTiles; x++) {
        for (int y = 0; y < verticalTiles; y++) {
            uiDrawRect(&ctx, getScreenX(ui, x), getScreenY(ui, y), squareSize, squareSize, gridColor);
        }
    }
    uiFillRect(&ctx, getScreenX(ui, 0), getScreenY(ui, 0), squareSize, squareSize, gridColor);
    uiFillRect(&ctx, getScreenX(ui, horisontalTiles-1), getScreenY(ui, 0), squareSize, squareSize, gridColor);
    uiFillRect(&ctx, getScreenX(ui, 0), getScreenY(ui, verticalTiles-1), squareSize, squareSize, gridColor);
    uiFillRect(&ctx, getScreenX(ui, horisontalTiles-1), getScreenY(ui, verticalTiles-1), squareSize, squareSize, gridColor);
}

void drawHero(Game *game, const Ui *ui) {
    Hero *hero = &game->hero[game->currentHero];
    UiContext ctx;
    uiContextSet(ui,
        &ctx,
        (uiWidth(ui) - (horisontalTiles * squareSize)) / 2 + horisontalTiles * tileSize + 8,
        (uiHeight(ui) - (verticalTiles * squareSize)) / 2,
        uiWidth(ui),
        uiHeight(ui));

    const int lifeSize = 8;
    for (int i = 0; i < hero->life; i++) {
        uiFillRect(&ctx, i * lifeSize, 0, lifeSize-1, lifeSize, lifeColor); 
    }
}

void gameRender(void *userData, const Ui *ui) {
    //drawBoard(ui);
    Game *game = (Game*)userData;
    //drawHero(game, ui);
    UiContext ctx;
    uiContextSetDefault(ui, &ctx);
    uiDrawTile(&ctx, 0, 0, game->tiles.foo);
}
