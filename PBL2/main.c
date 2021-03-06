#include "General.h"
#include "Menu.h"
#include "PathFinding.h"
#include "Editor.h"
/*
Programa zein ataletan dagoen zehazteko. Honen arabera funtzionalitatea eta pantailaratzen dena aldatu.
*/
int clientState = CLIENT_MAIN_MENU;
int sim_diags = 1;
int sim_alg = 1;

/*
Programaren funtzio nagusia. 
[1] Programarentzat beharrezkoak diren aldagaiak hasieratu
[2] SDL liburutegiak hasieratu, erroreak maneiatu.
[3] Programaren while nagusia. Ziklo bakoitzean programaren logika exekutatu eta pantaila eguneratu.
[4] SDL liburutegiak eta programa itxi.
*/
int main(int argc, char* argv[]){
    SDL_Window* win = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* fileSurface = NULL, * menuSurface = NULL;
    SDL_Texture* fileTexture = NULL, * menuTexture = NULL;

    int running = init(&win, &renderer);
    int working = 0, loadedMap = 0;
    if (running) {
        //Start exec
        initNodes(sim_diags);
        for (int i = 0; i < 322; i++) *(KEYS + i) = 0;
        if (loadMenu(&menuSurface)) {
            menuTexture = SDL_CreateTextureFromSurface(renderer, menuSurface);
            if (!menuTexture) {
                free(menuSurface);
                printf("Errorea menuaren textura sortzean. Aplikazioa ixten.\n");
                running = 0;
            }
            SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
        }
        else {
            printf("Errorea menua kargatzean. Aplikazioa ixten.\n");
            running = 0;
        }
    }
    else printf("Ezin izan da hasieratu.");

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = 0;
                break;
            }
            else eventHandler(e);
        }
        if (!running) break;

        switch (clientState) {
        case CLIENT_MAIN_MENU:
            running = mainMenu(renderer, &fileSurface, &clientState);
            if (fileSurface) {
                SDL_DestroyTexture(fileTexture);
                fileTexture = SDL_CreateTextureFromSurface(renderer, fileSurface);
                if (!loadedMap) {
                    loadMap(fileSurface);
                    loadedMap = 1;
                }
            }
            break;
        case CLIENT_EDITOR:
            if (!fileSurface) {
                printf("Surface-rik ez. Menura bueltatzen.\n");
                clientState = CLIENT_MAIN_MENU;
            } else {
                switch (editor(&fileSurface, renderer, &clientState)) {
                case 1:
                    SDL_DestroyTexture(fileTexture);
                    fileTexture = SDL_CreateTextureFromSurface(renderer, fileSurface);
                    break;
                case 2:
                    running = 0;
                    break;
                }
            }
            renderEditor(renderer, fileTexture);
            break;
        case CLIENT_SIM:
            if (!working) {
                initNodes(sim_diags);
                loadMap(fileSurface);
                if (!findPath(renderer, sim_alg, sim_diags))
                    working = 0;
                else working = 1;
            }
            SDL_Rect rect = { 0, MENU_HEIGHT, WIDTH, HEIGHT };
            SDL_RenderCopy(renderer, fileTexture, NULL, &rect);
            if (working) {
                printfPath(renderer);
                if (!robotSim(renderer)) {
                    working = 0;
                    clientState = CLIENT_EDITOR;
                }
            }
            else clientState = CLIENT_EDITOR;
            break;
        }
        SDL_RenderPresent(renderer);
    }
	return 0;
}

/*
SDL liburutegiak hasieratu eta erroreak maneiatzeko.
[1] SDL oinarrizko liburutegia hasieratu. Pantaila eta renderra sortu.
[2] SDL Image hasieratu PNG formatoarekin.
[3] SDL TTF hasieratu.
*/
int init(SDL_Window** win, SDL_Renderer** renderer) {
    int success = 1;
    int imgFlags = IMG_INIT_PNG;

    //Hasieratu SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL ez da hasieratu. SDL_Error: %s\n", SDL_GetError());
        success = 0;
    }
    else {
        //Zabaldu leihoa
        *win = SDL_CreateWindow("Regadier", 500, 60, WIDTH, HEIGHT + MENU_HEIGHT, 0);
        if (*win == NULL) {
            printf("Ez da leihoa sortu. SDL_Error: %s\n", SDL_GetError());
            success = 0;
        }
        else {
            *renderer = SDL_CreateRenderer(*win, -1, SDL_RENDERER_PRESENTVSYNC);
            SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_BLEND);
        }
    }

    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image ez da hasieratu. SDL_image Error: %s\n", IMG_GetError());
        success = 0;
    }

    if (TTF_Init() < 0) {
        printf("SDL_ttf ez da hasieratu. SDL_ttf Error: %s\n", TTF_GetError());
        success = 0;
    }
    return success;
}