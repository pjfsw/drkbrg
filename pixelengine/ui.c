#include "ui.h"

#include <SDL3_image/SDL_image.h>

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

void uiInit(Ui *ui, SDL_Renderer *renderer, int w, int h) {
    memset(ui, 0, sizeof(Ui));
    if (w < 1) {
        w = 1;
    } 
    if (h < 1) {
        h = 1;
    }

    ui->renderer = renderer;
    ui->w = w;
    ui->h = h;
    ui->target = createTexture(renderer, w, h);
}

void uiDestroy(Ui *ui) {
    SDL_DestroyTexture(ui->target);
}


int uiWidth(const Ui *ui) {
    return ui->w;
}

int uiHeight(const Ui *ui) {
    return ui->h;
}

TilesheetId uiLoadTilesheet(Ui *ui, const char *path) {
    if (ui->tilesheetCount >= MAX_TILESHEETS) {
        return -1;
    }
    SDL_Texture *tex = IMG_LoadTexture(ui->renderer, path);
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    TilesheetId id = ui->tilesheetCount++;
    ui->tilesheets[id] = tex;
    return id;
}

TileId uiCreateTile(Ui *ui, TilesheetId tilesheetId, int x, int y, int w, int h) {
    if ((tilesheetId < 0) || (tilesheetId >= ui->tilesheetCount) || (x < 0) || (y < 0) || (w < 0) || (h < 0) ||
        (ui->tileCount >= MAX_TILES)) {
        return -1;
    }
    float texw, texh;
    SDL_GetTextureSize(ui->tilesheets[tilesheetId], &texw, &texh);
    if ((x >= texw) || (y >= texh)) {
        return -1;
    }      
    if ((x+w) >= texw) {
        w = texw-x;        
    }
    if ((y+h) >= texh) {
        h = texh-h;
    }
    int tileId = ui->tileCount++;
    Tile *tile = &ui->tiles[tileId];
    tile->tilesheetId = tilesheetId;
    tile->cut.x = x;
    tile->cut.y = y;
    tile->cut.w = w;
    tile->cut.h = h;   
    return tileId; 
}

void uiContextSetDefault(const Ui *ui, UiContext *ctx) {
    ctx->ui = ui;
    ctx->useCut = false;
}

void uiContextSet(const Ui *ui, UiContext *ctx, int x, int y, int w, int h) {
    ctx->ui = ui;
    ctx->useCut = true;
    ctx->cut.x = x;
    ctx->cut.y = y;
    ctx->cut.w = w;
    ctx->cut.h = h;
}

static void setContext(const UiContext *ctx) {
    if (ctx->useCut) {
        SDL_SetRenderClipRect(ctx->ui->renderer, &ctx->cut);
    } else {
        SDL_SetRenderClipRect(ctx->ui->renderer, NULL);
    }
}

static inline int getX(const UiContext *ctx, int x) {
    if (ctx->useCut) {
        return ctx->cut.x + x;
    } else {
        return x;
    }
}

static inline int getY(const UiContext *ctx, int y) {
    if (ctx->useCut) {
        return ctx->cut.y + y;
    } else {
        return y;
    }
}

void uiDrawTile(const UiContext *ctx, int x, int y, TileId tile) {
    const Ui *ui = ctx->ui;

    if ((tile < 0) || (tile >= ui->tileCount)) {
        return;
    }
}

void uiDrawRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color, bool fill) {
    const Ui *ui = ctx->ui;
    setContext(ctx);
    SDL_FRect rect = {
        .x = getX(ctx, x),
        .y = getY(ctx, y),
        .w = w,
        .h = h
    };
    SDL_SetRenderDrawColor(ui->renderer, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);    
    if (fill) {
        SDL_RenderFillRect(ui->renderer, &rect);
    } else {
        SDL_RenderRect(ui->renderer, &rect);
    }
}

void uiFillRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color) {
    uiDrawRect(ctx, x, y, w, h, color, true);
}

void uiBeginRender(Ui *ui) {
    SDL_SetRenderTarget(ui->renderer, ui->target);
}

void uiFinalizeRender(Ui *ui) {
    // Reset target to the main window
    SDL_SetRenderTarget(ui->renderer, NULL);

    int winW, winH;
    SDL_GetRenderOutputSize(ui->renderer, &winW, &winH);

    float scaleX = (float)winW / ui->w;
    float scaleY = (float)winH / ui->h;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    SDL_FRect destRect;
    destRect.w = ui->w * scale;
    destRect.h = ui->h * scale;
    destRect.x = (winW - destRect.w) / 2.0f;
    destRect.y = (winH - destRect.h) / 2.0f;

    // Render our 'virtual' screen onto the actual window 
    SDL_RenderTexture(ui->renderer, ui->target, NULL, &destRect);
}

