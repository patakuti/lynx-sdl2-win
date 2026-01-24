/* Test SDL2 TTF rendering for CJK characters
 * This program measures actual rendering values to debug the half-cut issue
 */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main(int argc, char *argv[])
{
    const char *font_path;
    int font_index = 0;
    int font_size = 18;
    TTF_Font *font;
    SDL_Color white = {255, 255, 255, 255};

    /* Test characters: ASCII 'A', Hiragana 'あ', Kanji '日' */
    Uint16 test_chars[][2] = {
        {'A', 0},           /* ASCII */
        {0x3042, 0},        /* あ (Hiragana) */
        {0x65E5, 0},        /* 日 (Kanji) */
        {0x672C, 0},        /* 本 (Kanji) */
        {0xFF21, 0},        /* Ａ (Fullwidth A) */
    };
    const char *names[] = {"A (ASCII)", "あ (Hiragana)", "日 (Kanji)", "本 (Kanji)", "Ａ (Fullwidth)"};
    int num_chars = sizeof(test_chars) / sizeof(test_chars[0]);

    setlocale(LC_ALL, "");

    /* Get font from environment or use default */
    font_path = getenv("PDC_FONT");
    if (!font_path) {
        font_path = "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc";
    }
    if (getenv("PDC_FONT_INDEX")) {
        font_index = atoi(getenv("PDC_FONT_INDEX"));
    }
    if (getenv("PDC_FONT_SIZE")) {
        font_size = atoi(getenv("PDC_FONT_SIZE"));
    }

    printf("=== SDL2 TTF Rendering Test ===\n");
    printf("Font: %s\n", font_path);
    printf("Font Index: %d\n", font_index);
    printf("Font Size: %d\n\n", font_size);

    /* Initialize SDL and TTF */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    /* Open font */
    font = TTF_OpenFontIndex(font_path, font_size, font_index);
    if (!font) {
        fprintf(stderr, "TTF_OpenFontIndex failed: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    /* Get font metrics */
    int font_height = TTF_FontHeight(font);
    int font_ascent = TTF_FontAscent(font);
    int font_descent = TTF_FontDescent(font);
    int font_lineskip = TTF_FontLineSkip(font);

    printf("Font Metrics:\n");
    printf("  Height: %d\n", font_height);
    printf("  Ascent: %d\n", font_ascent);
    printf("  Descent: %d\n", font_descent);
    printf("  Line Skip: %d\n\n", font_lineskip);

    /* Measure 'W' for cell width (PDCurses uses this) */
    int w_minx, w_maxx, w_miny, w_maxy, w_advance;
    TTF_GlyphMetrics(font, 'W', &w_minx, &w_maxx, &w_miny, &w_maxy, &w_advance);
    printf("'W' Glyph Metrics (used for pdc_fwidth):\n");
    printf("  minx=%d, maxx=%d, miny=%d, maxy=%d, advance=%d\n",
           w_minx, w_maxx, w_miny, w_maxy, w_advance);
    printf("  Glyph width (maxx-minx): %d\n\n", w_maxx - w_minx);

    /* PDCurses cell width calculation (from pdcsetsc.c) */
    int pdc_fwidth = w_maxx - w_minx + 1;  /* This is how PDCurses calculates it */
    printf("Estimated pdc_fwidth: %d\n\n", pdc_fwidth);

    printf("Character Rendering Tests:\n");
    printf("==========================\n");

    for (int i = 0; i < num_chars; i++) {
        SDL_Surface *surface;
        int minx, maxx, miny, maxy, advance;

        /* Get glyph metrics */
        TTF_GlyphMetrics(font, test_chars[i][0], &minx, &maxx, &miny, &maxy, &advance);

        /* Render the character */
        surface = TTF_RenderUNICODE_Blended(font, test_chars[i], white);

        printf("%s (U+%04X):\n", names[i], test_chars[i][0]);
        printf("  Glyph Metrics: minx=%d, maxx=%d, miny=%d, maxy=%d, advance=%d\n",
               minx, maxx, miny, maxy, advance);
        printf("  Glyph width (maxx-minx): %d\n", maxx - minx);

        if (surface) {
            printf("  Rendered Surface: w=%d, h=%d\n", surface->w, surface->h);
            printf("  Surface vs pdc_fwidth: %.2f cells\n",
                   (float)surface->w / pdc_fwidth);

            /* Check if this is a wide character */
            int is_cjk = (test_chars[i][0] >= 0x1100 &&
                         (test_chars[i][0] <= 0x115f ||
                          (test_chars[i][0] >= 0x2e80 && test_chars[i][0] <= 0x4dbf) ||
                          (test_chars[i][0] >= 0x4e00 && test_chars[i][0] <= 0xa4cf) ||
                          (test_chars[i][0] >= 0xac00 && test_chars[i][0] <= 0xd7a3) ||
                          (test_chars[i][0] >= 0xf900 && test_chars[i][0] <= 0xfaff) ||
                          (test_chars[i][0] >= 0xff00 && test_chars[i][0] <= 0xff60)));

            printf("  Is CJK wide: %s\n", is_cjk ? "YES" : "NO");
            printf("  Expected cells: %d\n", is_cjk ? 2 : 1);
            printf("  Expected width: %d pixels\n", is_cjk ? pdc_fwidth * 2 : pdc_fwidth);

            if (is_cjk && surface->w > pdc_fwidth) {
                printf("  *** ISSUE: Surface width (%d) > 1 cell (%d)\n",
                       surface->w, pdc_fwidth);
                printf("  *** This character needs %d pixels but only %d allocated\n",
                       surface->w, pdc_fwidth);
            }

            SDL_FreeSurface(surface);
        } else {
            printf("  Rendering failed: %s\n", TTF_GetError());
        }
        printf("\n");
    }

    /* Cleanup */
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
