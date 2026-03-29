#include "game.h"

static const int horisontalTiles = 13;
static const int verticalTiles = 10;
static const int tileSize = 32;
static const int squareSize = tileSize+1;
static const uint32_t gridColor = 0x282422ff;
static const uint32_t lifeColor = 0xcc1111ff;
static const uint32_t textColor = 0xffffffff;

static void updateSundialString(Game *game) {
    memset(game->sundialString, 0, sizeof(game->sundialString));
    SDL_itoa(game->sundial, game->sundialString, 10);
}

static void initUiElements(Game *game, Ui *ui) {
    const char *txt[] = {"FOO", "", "BAR BAZ", 0};
    Graphics *g = &game->g;
    g->foo = uiCreateTextboxTile(ui, txt, 96,128, 0xffffffff, 0xffffffff, 0x383432ff, true);
    int bx = (uiWidth(ui) - (horisontalTiles * squareSize)) / 2;
    int bw = horisontalTiles * tileSize + 1;
    int by = (uiHeight(ui) - (verticalTiles * squareSize)) / 2;
    int bh = verticalTiles * tileSize + 1;
    uiContextSet(ui, &g->boardCtx, bx, by, bw, bh);
    uiContextSet(ui, &g->rPanelCtx, bx + bw + 8, by, uiWidth(ui), uiHeight(ui));
}

void gameInit(void *userData, Ui *ui) {
    Game *game = (Game*)userData;
    memset(game, 0, sizeof(Game));
    for (int i = 0; i < 4; i++) {
        game->hero[i].npc = true;
        game->hero[i].life = i + 7;
    }       
    game->sundial = 1;
    updateSundialString(game);
    initUiElements(game, ui);
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

void drawHero(Game *game, const Ui *ui) {
    Hero *hero = &game->hero[game->currentHero];
    UiContext ctx;

    const int lifeSize = 8;
    for (int i = 0; i < hero->life; i++) {
        uiFillRect(&ctx, i * lifeSize, 0, lifeSize-1, lifeSize, lifeColor); 
    }
}

static void drawRightPanel(Game *game, const Ui *ui) {
    int offset = (uiWidth(ui) - (horisontalTiles * squareSize)) / 2 + horisontalTiles * tileSize + 8;
    
    UiContext *ctx = &game->g.rPanelCtx;
    
    uiWrite(ctx, "TURN   /26", 0, 0, textColor);
    uiWrite(ctx, game->sundialString, 40, 0, textColor);
    uiDrawTile(ctx, 0, 16, game->g.foo);

}
void gameRender(void *userData, const Ui *ui) {
    Game *game = (Game*)userData;
    drawBoard(&game->g, ui);
    drawHero(game, ui);
    drawRightPanel(game, ui);
}
