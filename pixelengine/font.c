#include <SDL3/SDL.h>
#include <stdio.h>

#include "font.h"
#include "nesfont.h"

static SDL_Texture *textureCreate(SDL_Renderer *renderer, int w, int h) {
    return SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );
}

void fontInit(Font *font, SDL_Renderer *renderer) {
    font->renderer = renderer;
    uint32_t fg = 0xffffffff;  // white
    uint32_t bg = 0x00000000;  // transparent or black
    uint32_t pixels[FONT_WIDTH*FONT_HEIGHT];    // RGBA8888 output        
    for (int i = 0 ; i < 256; i++) {
        for (int y = 0; y < FONT_HEIGHT; y++) {
            uint8_t row = nes_font[i*FONT_HEIGHT+y];
            for (int x = 0; x < 8; x++) {
                int bit = (row >> (7 - x)) & 1;
                pixels[y * 8 + x] = bit ? fg : bg;
            }
        }        
        font->font[i] = textureCreate(renderer, FONT_WIDTH, FONT_HEIGHT);
        SDL_UpdateTexture(font->font[i], NULL, pixels, FONT_WIDTH * sizeof(uint32_t));
    }
}

void fontDestroy(Font *font) {
    for (int i = 0; i < 256; i++) {
        SDL_DestroyTexture(font->font[i]);
    }
}

static void fontPutc(const Font *font, uint8_t c, int x, int y, uint32_t color) {
    SDL_Texture *t = font->font[c];
    uint32_t r = (color >> 24) & 0xff;
    uint32_t g = (color >> 16) & 0xff;
    uint32_t b = (color >> 8) & 0xff;
    uint32_t a = color & 0xff;
    SDL_SetTextureColorMod(t, r, g, b);
    SDL_SetTextureAlphaMod(t, a);  // optional
    SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(t, SDL_SCALEMODE_PIXELART);
    SDL_FRect dst = {
        .x = x,
        .y = y,
        .w = FONT_SCALE * FONT_WIDTH,
        .h = FONT_SCALED_HEIGHT
    };

    SDL_RenderTexture(font->renderer, t, NULL, &dst);
}

inline void fontWrite(const Font *font, const char *str, int x, int y, uint32_t color) {
    for (int i = 0 ; i < strlen(str); i++) {
        fontPutc(font, str[i], x + i * FONT_SCALE * FONT_WIDTH, y, color);
    } 
}