#include "PathFinding.h"

/*
Hasierako menuaren funtzio nagusia. Botoiak sakatzen diren begiratu. Sakatzean funtzio desberdinei deitu.
*/
int mainMenu(SDL_Renderer* renderer, SDL_Surface** surface, int* clientState) {
    int running = 1;
    SDL_Rect btn_import = { 211, 318, 327, 108 }, btn_newFile = { 211, 318 + 41 + 108, 327, 108 }, btn_quit = { 211, 318 + 82 + 216, 327, 108 };
    if (MOUSE_CLICK) {
        if (checkButton(btn_quit)) {
            running = 0;
        }
        else if (KEYS[SDLK_i] || checkButton(btn_import)) {
            if (importMap(surface)) *clientState = CLIENT_EDITOR;
        }
        else if (KEYS[SDLK_n] || checkButton(btn_newFile)) {
            *surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, NULL, NULL, NULL, NULL);
            if (!*surface) return 0;
            SDL_FillRect(*surface, NULL, SDL_MapRGB((*surface)->format, 255, 255, 255));
            *clientState = CLIENT_EDITOR;
        }
    }

    return running;
}

/*
Sagua botoi baten barnean dagoen ala ez.
*/
int checkButton(SDL_Rect btn) {
    int success = 0, x, y;
    SDL_GetMouseState(&x, &y);
    if (x > btn.x &&
        x < btn.x + btn.w &&
        y > btn.y &&
        y < btn.y + btn.h) {
        success = 1;
    }
    return success;
}

/*
Hasierako menuaren irudia surface batera kargatu.
*/
int loadMenu(SDL_Surface** surface) {
    int loaded = 0;
    SDL_RWops* fp = SDL_RWFromFile("images/menus.png", "rb");
    if (fp) {
        SDL_Surface* s = IMG_LoadPNG_RW(fp);
        if (s) {
            if (*surface) SDL_free(*surface);
            *surface = s;
            loaded = 1;
        }
        SDL_RWclose(fp);
    }

    return loaded;
}