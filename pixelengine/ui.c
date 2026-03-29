#include <SDL3_image/SDL_image.h>
#include <stdio.h>

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
    fontInit(&ui->font, renderer);
}

void uiDestroy(Ui *ui) {
    fontDestroy(&ui->font);
    SDL_DestroyTexture(ui->target);
}


int uiWidth(const Ui *ui) {
    return ui->w;
}

int uiHeight(const Ui *ui) {
    return ui->h;
}

static inline void setColor(const Ui *ui, uint32_t color) {
    SDL_SetRenderDrawColor(ui->renderer, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff, color & 0xff);  
}

static TilesheetId addTileSheet(Ui *ui, SDL_Texture *tex) {
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    TilesheetId id = ui->tilesheetCount++;
    ui->tilesheets[id] = tex;
    return id;
}

TilesheetId uiLoadTilesheet(Ui *ui, const char *path) {
    if (ui->tilesheetCount >= MAX_TILESHEETS) {
        return -1;
    }
    SDL_Texture *tex = IMG_LoadTexture(ui->renderer, path);
    return addTileSheet(ui, tex);
}

static TileId createTile(Ui *ui, TilesheetId tilesheetId, int x, int y, int w, int h) {
    int tileId = ui->tileCount++;
    Tile *tile = &ui->tiles[tileId];
    tile->tilesheetId = tilesheetId;
    tile->cut.x = x;
    tile->cut.y = y;
    tile->cut.w = w;
    tile->cut.h = h;   
    return tileId; 
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
    return createTile(ui, tilesheetId, x, y, w, h);
}

TileId uiCreateTextboxTile(
    Ui *ui, const char *text, int w, int h, uint32_t textColor, uint32_t frameColor, uint32_t fillColor, bool fill) {
    if ((ui->tilesheetCount >= MAX_TILESHEETS) || (ui->tileCount >= MAX_TILES)) {
        return -1;
    }
    SDL_Texture *tex = createTexture(ui->renderer, w, h);
    TilesheetId tilesheetId = addTileSheet(ui, tex);
    SDL_SetRenderTarget(ui->renderer, tex);
    SDL_FRect rect = {
        .x = 0,
        .y = 0,
        .w = w-1,
        .h = h-1
    };    
    if (fill) {
        setColor(ui, fillColor);
        SDL_RenderFillRect(ui->renderer, &rect);
    }
    setColor(ui, frameColor);
    SDL_RenderRect(ui->renderer, &rect);
    int rows = 1;
    int i = 0 ;
    while (text[i] != 0) {
        if (text[i] == '\n') {
            rows++;
        }
        i++;
    };

    int ty = (h - rows * FONT_HEIGHT) / 2 ;
    int cpos = 0;
    for (int i = 0; i < rows; i++) {
        int start = cpos;
        while ((text[cpos] != 0) && (text[cpos] != '\n')) {
            cpos++;
        }
        char buf[100];
        memset(buf, 0, sizeof(buf));
        strncpy(buf, &text[start], cpos-start);

        int tx = (w - strlen(buf) * FONT_WIDTH) / 2;
        fontWrite(&ui->font, buf, tx, ty + i * FONT_HEIGHT, textColor);
        cpos++;
    }
    return createTile(ui, tilesheetId, 0, 0, w, h);
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

void uiDrawTile(const UiContext *ctx, int x, int y, TileId tileId) {
    const Ui *ui = ctx->ui;
    if ((tileId < 0) || (tileId >= ui->tileCount)) {
        return;
    }
    setContext(ctx);

    Tile *tile = &ui->tiles[tileId];    
    SDL_FRect srcRect = {
        .x = tile->cut.x,
        .y = tile->cut.y,
        .w = tile->cut.w,
        .h = tile->cut.h
    };
    SDL_FRect destRect = {
        .x = getX(ctx, x),
        .y = getY(ctx, y),
        .w = tile->cut.w,
        .h = tile->cut.h
    };
    SDL_Texture *tex = ui->tilesheets[tile->tilesheetId];
    SDL_RenderTexture(ui->renderer, tex, &srcRect, &destRect);
}

static inline void drawOrFillRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color, bool fill) {
    const Ui *ui = ctx->ui;
    setContext(ctx);
    SDL_FRect rect = {
        .x = getX(ctx, x),
        .y = getY(ctx, y),
        .w = w,
        .h = h
    };
    setColor(ui, color);
    if (fill) {
        SDL_RenderFillRect(ui->renderer, &rect);
    } else {
        SDL_RenderRect(ui->renderer, &rect);
    }
}

void uiDrawRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color) {
    drawOrFillRect(ctx, x, y, w, h, color, false);
}

void uiFillRect(const UiContext *ctx, int x, int y, int w, int h, uint32_t color) {
    drawOrFillRect(ctx, x, y, w, h, color, true);
}

void uiWrite(const UiContext *ctx, const char *text, int x, int y, uint32_t color) {
    const Ui *ui = ctx->ui;
    setContext(ctx);
    fontWrite(&ui->font, text, getX(ctx, x), getY(ctx, y), color);
}


void uiBeginRender(Ui *ui) {
    SDL_SetRenderTarget(ui->renderer, ui->target);
    SDL_SetRenderDrawColor(ui->renderer, 0, 0, 0, 255); /* Dark background */
    SDL_RenderClear(ui->renderer);
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

