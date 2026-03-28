#include "ui.h"

static SDL_Texture *createTexture(SDL_Renderer *renderer, int w, int h) {
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    return texture;
}

void uiInit(Ui *ui, SDL_Renderer *renderer) {
    memset(ui, 0, sizeof(Ui));
    ui->renderer = renderer;
    ui->target = createTexture(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void uiDestroy(Ui *ui) {
    SDL_DestroyTexture(ui->target);
}

void uiDrawTile(Ui *ui, int x, int y, int tile) {
    // TODO use preloaded pngs
    SDL_FRect rect = {
        .x = x * TILE_SIZE,
        .y = y * TILE_SIZE,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };
    SDL_SetRenderDrawColor(ui->renderer, 255,0,255,255);
    SDL_RenderRect(ui->renderer, &rect);
}

void uiBeginRender(Ui *ui) {
    SDL_SetRenderTarget(ui->renderer, ui->target);
}

void uiFinalizeRender(Ui *ui) {
    // Reset target to the main window
    SDL_SetRenderTarget(ui->renderer, NULL);

    int winW, winH;
    SDL_GetRenderOutputSize(ui->renderer, &winW, &winH);

    float scaleX = (float)winW / SCREEN_WIDTH;
    float scaleY = (float)winH / SCREEN_HEIGHT;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    SDL_FRect destRect;
    destRect.w = SCREEN_WIDTH * scale;
    destRect.h = SCREEN_HEIGHT * scale;
    destRect.x = (winW - destRect.w) / 2.0f;
    destRect.y = (winH - destRect.h) / 2.0f;

    // Render our 'virtual' screen onto the actual window 
    SDL_RenderTexture(ui->renderer, ui->target, NULL, &destRect);
}

