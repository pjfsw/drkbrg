#include "game.h"

void gameInit(Game *game) {
    memset(game, 0, sizeof(Game));
}

void gameUpdate(void *userData) {
    Game *game = (Game*)userData;
}

void gameRender(void *userData, Ui *ui) {
    Game *game = (Game*)userData;
    uiDrawTile(ui, 0, 0, 0);
}
