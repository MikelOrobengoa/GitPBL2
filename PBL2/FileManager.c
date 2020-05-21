#include "PathFinding.h"

/*
Surface bat PNG irudi bezala exportatzeko.
Fitxategiaren izena eta direktorioa ezartzeko aukera WIP
*/
int exportMap(SDL_Surface* surface) {
    char* path = "images/export.png";
    IMG_SavePNG(surface, path);
    return 0;
}

/*
Irudi bat inportatzeko pantailara arrastratuaz.
[1] Ebentoak begiratu ESC tekla sakatuaz irten edo fitxategia arrastratu arte.
[2] Fitxategiaren helbidea lortutakoan surface batera bihurtu eta beharrezko memoriak askatu.
*/
int importMap(SDL_Renderer* renderer, SDL_Surface** surface) {
    char fileDir[256];
    int loaded = 0;
    SDL_Event e;
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    while (!loaded)
        while (SDL_PollEvent(&e)) {
            eventHandler(e);
            if (KEYS[SDLK_ESCAPE] || e.type == SDL_QUIT) return 0;
            else if (e.type == SDL_DROPFILE) {
                strcpy(fileDir, e.drop.file);
                printf("File dropped on window: %s", fileDir);
                SDL_RWops* fp = SDL_RWFromFile(fileDir, "rb");
                if (fp) {
                    SDL_Surface* s = IMG_LoadPNG_RW(fp);
                    if (s) {
                        if (*surface) SDL_free(*surface);
                        *surface = s;
                        loaded = 1;
                    }
                    SDL_RWclose(fp);
                }
                break;
            }

        }
    return loaded;
}