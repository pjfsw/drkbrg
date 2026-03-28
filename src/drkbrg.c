#include "game.h"
#include "pixelengine.h"

int main(int argc, char *argv[]) {
    Game game;
    gameInit(&game);
    PixelEngine pe;
    peInit(&pe, 640, 360, "Drkbrg");
    peRun(&pe, gameUpdate, gameRender, &game);
    peDestroy(&pe);
}