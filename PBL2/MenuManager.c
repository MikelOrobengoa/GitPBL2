#include "General.h"
#include "Menu.h"
#include "FileManager.h"
#include "Editor.h"

/*
Hasierako menuaren funtzio nagusia. Botoiak sakatzen diren begiratu. Sakatzean funtzio desberdinei deitu.
*/
int mainMenu(SDL_Renderer* renderer, SDL_Surface** surface, int* clientState) {
    int running = 1;
    static int button = -1;
    SDL_Rect btn_import = { 221, 476, 342, 94 }, btn_newFile = { 221, 340, 342, 94 }, btn_quit = { 221, 617, 342, 94 };
    static SDL_Texture* TexMenu = NULL;
    if (!TexMenu) {
        SDL_Surface* surfMenu = NULL;
        if (loadMenu(&surfMenu)) {
            TexMenu = SDL_CreateTextureFromSurface(renderer, surfMenu);
            SDL_FreeSurface(surfMenu);
        }
    }

    if (MOUSE_CLICK) {
        if (checkButton(btn_quit)) {
            SDL_RenderCopy(renderer, TexMenu, NULL, NULL);
            if(button == 0) drawIMG(renderer, "images/exit_click.png", btn_quit);
            button = 0;
        }
        else if (checkButton(btn_import)) {
            SDL_RenderCopy(renderer, TexMenu, NULL, NULL);
            if (button == 1)drawIMG(renderer, "images/import_click.png", btn_import);
            button = 1;
        }
        else if (checkButton(btn_newFile)) {
            SDL_RenderCopy(renderer, TexMenu, NULL, NULL);            
            if (button == 2) drawIMG(renderer, "images/new_click.png", btn_newFile);
            button = 2;
        }
    }
    else {
        if (button == 0 && checkButton(btn_quit)) {
            running = 0;
        }
        else if (button == 1 && checkButton(btn_import)) {
            switch (importMap_menu(surface, renderer)) {
            case 1:
                *clientState = CLIENT_EDITOR;
                break;
            case -1:
                running = 0;
            }
        }
        else if (button == 2 && checkButton(btn_newFile)) {
            *surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
            if (!*surface) return 0;
            SDL_FillRect(*surface, NULL, SDL_MapRGB((*surface)->format, 255, 255, 255));
            *clientState = CLIENT_EDITOR;
        }
        else SDL_RenderCopy(renderer, TexMenu, NULL, NULL);
        button = -1;
    }
    SDL_RenderPresent(renderer);    
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

void drawIMG(SDL_Renderer* renderer, char* path, SDL_Rect btn) {
    SDL_Surface* surf = NULL;
    loadIMG(&surf, path);
    if (surf) {
        SDL_Texture* Tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        SDL_RenderCopy(renderer, Tex, NULL, &btn);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(Tex);
    }
}