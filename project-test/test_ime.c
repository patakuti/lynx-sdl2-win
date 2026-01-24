/*
 * test_ime.c - SDL2 IME テストプログラム
 * Windows で日本語入力 (IME) の動作を確認する
 */

#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FONT_SIZE 24

static TTF_Font *font = NULL;
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

/* 入力テキストバッファ */
static char input_text[1024] = "";
static char composition_text[256] = "";
static int composition_cursor = 0;
static int composition_selection_len = 0;

void render_text(const char *text, int x, int y, SDL_Color color) {
    if (!text || !text[0]) return;

    SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, color);
    if (surface) {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture) {
            SDL_Rect dest = {x, y, surface->w, surface->h};
            SDL_RenderCopy(renderer, texture, NULL, &dest);
            SDL_DestroyTexture(texture);
        }
        SDL_FreeSurface(surface);
    }
}

void render_screen(void) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color cyan = {0, 255, 255, 255};
    SDL_Color gray = {128, 128, 128, 255};

    /* 背景クリア */
    SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
    SDL_RenderClear(renderer);

    /* タイトル */
    render_text("SDL2 IME Test - Type Japanese text", 20, 20, white);
    render_text("Press ESC to quit", 20, 50, gray);

    /* 確定済みテキスト */
    render_text("Input:", 20, 120, gray);
    if (input_text[0]) {
        render_text(input_text, 100, 120, white);
    }

    /* 変換中テキスト (composition) */
    render_text("Composition:", 20, 160, gray);
    if (composition_text[0]) {
        render_text(composition_text, 150, 160, yellow);

        char info[128];
        snprintf(info, sizeof(info), "(cursor=%d, sel_len=%d)",
                 composition_cursor, composition_selection_len);
        render_text(info, 150, 190, cyan);
    }

    /* 入力領域の枠 */
    SDL_Rect input_rect = {18, 240, 764, 40};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &input_rect);

    /* 入力領域にテキスト表示 */
    char display[1280];
    snprintf(display, sizeof(display), "%s%s", input_text, composition_text);
    if (display[0]) {
        render_text(display, 25, 248, white);
    }

    /* IME入力位置を設定 */
    SDL_Rect ime_rect = {25, 248, 10, FONT_SIZE};
    if (input_text[0]) {
        int w, h;
        TTF_SizeUTF8(font, input_text, &w, &h);
        ime_rect.x = 25 + w;
    }
    SDL_SetTextInputRect(&ime_rect);

    /* デバッグ情報 */
    char debug[256];
    snprintf(debug, sizeof(debug), "IME Rect: x=%d, y=%d, w=%d, h=%d",
             ime_rect.x, ime_rect.y, ime_rect.w, ime_rect.h);
    render_text(debug, 20, 500, gray);

    SDL_bool text_input_active = SDL_IsTextInputActive();
    snprintf(debug, sizeof(debug), "TextInput Active: %s",
             text_input_active ? "YES" : "NO");
    render_text(debug, 20, 530, text_input_active ? cyan : yellow);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    SDL_Event event;
    int running = 1;

    (void)argc;
    (void)argv;

    /* SDL初期化 */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    /* IME UI表示を有効化 */
    SDL_bool hint_result = SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    printf("SDL_SetHint(SDL_HINT_IME_SHOW_UI, \"1\") = %s\n",
           hint_result ? "true" : "false");

    /* SDL_ttf初期化 */
    if (TTF_Init() < 0) {
        fprintf(stderr, "TTF_Init failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    /* ウィンドウ作成 */
    window = SDL_CreateWindow("SDL2 IME Test",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    /* レンダラー作成 */
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    /* フォント読み込み */
    const char *font_path = "C:\\Windows\\Fonts\\msgothic.ttc";
    font = TTF_OpenFont(font_path, FONT_SIZE);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont failed: %s\n", SDL_GetError());
        fprintf(stderr, "Trying meiryo.ttc...\n");
        font = TTF_OpenFont("C:\\Windows\\Fonts\\meiryo.ttc", FONT_SIZE);
    }
    if (!font) {
        fprintf(stderr, "Could not load any font\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    /* テキスト入力開始 */
    SDL_StartTextInput();
    printf("SDL_StartTextInput() called\n");
    printf("SDL_IsTextInputActive() = %s\n",
           SDL_IsTextInputActive() ? "true" : "false");

    /* メインループ */
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    /* バックスペース処理 */
                    size_t len = strlen(input_text);
                    if (len > 0) {
                        /* UTF-8の最後の文字を削除 */
                        while (len > 0 && (input_text[len-1] & 0xC0) == 0x80) {
                            input_text[--len] = '\0';
                        }
                        if (len > 0) {
                            input_text[--len] = '\0';
                        }
                    }
                }
                printf("KEYDOWN: sym=%d, scancode=%d\n",
                       event.key.keysym.sym, event.key.keysym.scancode);
                break;

            case SDL_TEXTEDITING:
                /* IME変換中テキスト */
                strncpy(composition_text, event.edit.text, sizeof(composition_text) - 1);
                composition_text[sizeof(composition_text) - 1] = '\0';
                composition_cursor = event.edit.start;
                composition_selection_len = event.edit.length;
                printf("TEXTEDITING: text=\"%s\", start=%d, length=%d\n",
                       event.edit.text, event.edit.start, event.edit.length);
                break;

            case SDL_TEXTINPUT:
                /* 確定テキスト */
                strncat(input_text, event.text.text,
                        sizeof(input_text) - strlen(input_text) - 1);
                composition_text[0] = '\0';
                composition_cursor = 0;
                composition_selection_len = 0;
                printf("TEXTINPUT: text=\"%s\"\n", event.text.text);
                break;
            }
        }

        render_screen();
        SDL_Delay(16);
    }

    /* 後片付け */
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
