#include "game.h"

void gameInit(Game *game) {
    memset(game, 0, sizeof(Game));
}

void gameUpdate(Game *game) {
}

void gameRender(Game *game, Ui *ui) {
    uiDrawTile(ui, 0, 0, 0);
}
