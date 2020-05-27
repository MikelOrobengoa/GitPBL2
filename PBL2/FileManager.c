#include "PathFinding.h"

/*
Surface bat PNG irudi bezala exportatzeko.
Fitxategiaren izena eta direktorioa ezartzeko aukera WIP
*/
int exportMap(SDL_Surface* surface) {
    char path[128] = "images/export";
    obtainpath(path);
    IMG_SavePNG(surface, path);
    return 0;
}

/*
Surface-a exportatzean, aurretik exportatutako beste mapa bat ez borratzeko izen
berria bilatzen du exportatutako mapa guztiak eskuragarri egoteko
*/
void obtainpath(char* path) {
    char pathCopy[128], tmp[6];
    int running = 1, i = 1;
    FILE* fp;
    strcpy(pathCopy, path);
    strcat(pathCopy, ".png");

    while (running) {
        fp = fopen(pathCopy, "r");
        if (fp != NULL) {
            sprintf(tmp, "%d", i);
            strcpy(pathCopy, path);
            strcat(pathCopy, "_");
            strcat(pathCopy, tmp);
            strcat(pathCopy, ".png");
            fclose(fp);
            i++;
        }
        else running = 0;
    }
    strcpy(path, pathCopy);
}

/*
Irudi bat inportatzeko pantailara arrastratuaz.
[1] Ebentoak begiratu ESC tekla sakatuaz irten edo fitxategia arrastratu arte.
[2] Fitxategiaren helbidea lortutakoan surface batera bihurtu eta beharrezko memoriak askatu.
*/
int importMap(SDL_Surface** surface) {
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
                        SDL_Surface* lol = *surface;
                        if (*surface) SDL_FreeSurface(lol);
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