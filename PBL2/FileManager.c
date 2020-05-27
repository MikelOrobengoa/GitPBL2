#include "PathFinding.h"


/*
Surface bat PNG irudi bezala exportatzeko.
Fitxategiaren izena eta direktorioa ezartzeko aukera
*/
int exportMap(SDL_Surface* surface, SDL_Renderer* renderer) {
	char path[128] = "";
	int gorde = 1, position_path = 0;
	SDL_Rect rect = { 200, HEIGHT / 2 - 50, WIDTH - 400, 50 };

	SDL_bool done = SDL_FALSE;

	SDL_StartTextInput();
	SDL_SetTextInputRect(&rect);
	while (!done) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				done = SDL_TRUE;
				gorde = 0;
				break;
			case SDL_TEXTINPUT:
				if (position_path == strlen(path)) {
					strcat(path, event.text.text);
					position_path++;
				}
				else {
					moveright(position_path, path);
					char tmp[128];
					strcpy(tmp, event.text.text);
					path[position_path] = tmp[0];
					position_path++;
				}
				showpath(path, renderer, position_path);
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
					SDL_SetClipboardText(path);
				}
				else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
					char tmp[128];
					strcpy(tmp, path + position_path);
					path[position_path] = '\0';
					strcat(path, SDL_GetClipboardText());
					strcat(path, tmp);
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE) {
					if (strlen(path) > 0 && position_path == strlen(path)) {
						path[strlen(path) - 1] = '\0';
					}
					else if (position_path > 0){
						for (int i = position_path - 1; i < (signed)strlen(path); i++) {
							path[i] = path[i + 1];
						}
					}
					if(position_path > 0) position_path--;
				}
				else if (event.key.keysym.sym == SDLK_DELETE) {
					if (strlen(path) >= 0 && position_path < (signed)strlen(path)) {
						for (int i = position_path; i <= (signed)strlen(path); i++) {
							path[i] = path[i + 1];
						}
					}
				}
				else if (event.key.keysym.sym == SDLK_LEFT) {
					if (position_path > 0) position_path--;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					if (position_path < (signed)strlen(path)) position_path++;
				}
				else if (event.key.keysym.sym == SDLK_RETURN) {
					done = SDL_TRUE;
				}
				showpath(path, renderer, position_path);
				break;
			}
		}
	}
	SDL_StopTextInput();
	if (gorde && formatuegokia(path)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SAVE", "Exportatu da png fitxategia", NULL);
		IMG_SavePNG(surface, path);
	}
	else {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Ezin izan da exportatu fitxategia", NULL);
		gorde = 0;
	}

    return gorde;
}

void moveright(int start, char* path) {
	int position = strlen(path);

	while (position >= start) {
		path[position + 1] = path[position];
		position--;
	}
}

void showpath(char* path, SDL_Renderer* renderer, int position_path) {
	char tmp[128];
	strcpy(tmp, path);

	TTF_Init();
	TTF_Font* Verdana = TTF_OpenFont("verdana.ttf", 24);
	SDL_Color Black = { 0, 0, 0, 255 };

	int w, h, i = 0, moved = 0;
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Verdana, tmp, Black);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_QueryTexture(Message, NULL, NULL, &w, &h);
	while (w > WIDTH - 404) {
		if (position_path > i) {
			SDL_FreeSurface(surfaceMessage);
			surfaceMessage = TTF_RenderText_Solid(Verdana, tmp + i + 1, Black);
			i++;
		}
		else {
			tmp[strlen(tmp) - 1] = '\0';
			SDL_FreeSurface(surfaceMessage);
			surfaceMessage = TTF_RenderText_Solid(Verdana, tmp + i, Black);
		}
		SDL_DestroyTexture(Message);
		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_QueryTexture(Message, NULL, NULL, &w, &h);
	}
	TTF_Quit();

	int ptrPos = 202, wTMP;
	char tmp2[128];
	strcpy(tmp2, path + i);
	tmp2[position_path - i] = '\0';
	if (strlen(tmp2) != 0) {
		SDL_Surface* surfaceTMP = TTF_RenderText_Solid(Verdana, tmp2, Black);
		SDL_Texture* MessageTMP = SDL_CreateTextureFromSurface(renderer, surfaceTMP);
		SDL_QueryTexture(MessageTMP, NULL, NULL, &wTMP, &h);
		SDL_DestroyTexture(MessageTMP);
		SDL_FreeSurface(surfaceTMP);
		ptrPos = wTMP + 201;
	}

	SDL_Rect Message_rect = { 202, 200, w, h };
	SDL_Rect rect = { 200, 200, WIDTH - 400, 30 };
	SDL_Rect rect2 = { 198, 198, WIDTH - 400 + 4, 30 + 4 };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect2);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, ptrPos, 202, ptrPos, 228);
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_RenderPresent(renderer);

	TTF_CloseFont(Verdana);
	SDL_FreeSurface(surfaceMessage);
}

int formatuegokia(char* path) {
	if (strcmp(path + strlen(path) - 4, ".png") == 0) {
		return 1;
	}
	else return 0;
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