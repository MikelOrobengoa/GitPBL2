#include "General.h"
#include "FileManager.h"
#include "Editor.h"
#include "Menu.h"

/*
Surface bat PNG irudi bezala exportatzeko.
Fitxategiaren izena eta direktorioa ezartzeko aukera WIP
*/
int exportMap(SDL_Surface* surface, SDL_Renderer* renderer) {
	static char path[128] = "exports/";
	int gorde = 1, position_path = strlen(path), botoia = 2;
	SDL_Rect rect = { 200, HEIGHT / 2 - 50, WIDTH - 400, 50 };
	SDL_bool done = SDL_FALSE;
	SDL_Texture* pathMessage = NULL;
	SDL_Rect btn_Save = { WIDTH / 2 - 96 / 2, 347 + 100, 96, 26 };
	SDL_Rect btn_Exit = { WIDTH / 2 - 96 / 2, 384 + 100, 96, 26 };
	SDL_Rect btn_export = { 206, 3, 96, 26 };
	static SDL_Texture* Save = NULL, *Exit = NULL;
	if (!Save && !Exit) {
		loadTexture(&Save, renderer, "images/save_selected.png");
		loadTexture(&Exit, renderer, "images/exit_selected.png");
	}
	exportMenu(renderer);
	pathMessage = showpath(path, renderer, position_path, pathMessage);


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
				pathMessage = showpath(path, renderer, position_path, pathMessage);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (checkButton(btn_Save)) {
					SDL_RenderCopy(renderer, Save, NULL, &btn_Save);
					SDL_RenderPresent(renderer);
					botoia = 0;
				}
				else if (checkButton(btn_Exit)) {
					SDL_RenderCopy(renderer, Exit, NULL, &btn_Exit);
					SDL_RenderPresent(renderer);
					botoia = 1;
				}
				else if (checkButton(btn_export)) {
					botoia = 1;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (botoia == 0) {
					done = SDL_TRUE;
				}
				else if (botoia == 1) {
					done = SDL_TRUE;
					gorde = -1;
				}
				break;
			case SDL_KEYDOWN:
				keydown(event, path, &position_path, &gorde, &done);
				pathMessage = showpath(path, renderer, position_path, pathMessage);
				break;
			}
		}
	}
	SDL_StopTextInput();
	if (gorde == 1) {
		int formatua = formatuegokia(path);
		if (formatua) {
			gorde = IMG_SavePNG(surface, path);
			if (gorde == 0)gorde = 1;
		}
		else gorde = 0;
	}
	else if (gorde == 0) gorde = 2;
	else if (gorde == -1) gorde = 3;


	SDL_DestroyTexture(pathMessage);

	return gorde;
}

void keydown(SDL_Event event, char* path, int *position_path, int* gorde, SDL_bool* done) {
	char tmp[128];

	switch (event.key.keysym.sym) {
	case SDLK_c:
		if (SDL_GetModState() & KMOD_CTRL) {
			SDL_SetClipboardText(path);
		}
		break;
	case SDLK_v:
		if (SDL_GetModState() & KMOD_CTRL) {
			strcpy(tmp, path + *position_path);
			path[*position_path] = '\0';
			strcat(path, SDL_GetClipboardText());
			strcat(path, tmp);
			*position_path += strlen(SDL_GetClipboardText());
		}
		break;
	case SDLK_BACKSPACE:
		if (strlen(path) > 0 && *position_path == strlen(path)) {
			path[strlen(path) - 1] = '\0';
		}
		else if (*position_path > 0) {
			for (int i = *position_path - 1; i < (signed)strlen(path); i++) {
				path[i] = path[i + 1];
			}
		}
		if (*position_path > 0) (*position_path)--;
		break;
	case SDLK_DELETE:
		if (strlen(path) >= 0 && *position_path < (signed)strlen(path)) {
			for (int i = *position_path; i <= (signed)strlen(path); i++) {
				path[i] = path[i + 1];
			}
		}
		break;
	case SDLK_LEFT:
		if (*position_path > 0) (*position_path)--;
		break;
	case SDLK_RIGHT:
		if (*position_path < (signed)strlen(path)) (*position_path)++;
		break;
	case SDLK_RETURN:
		*done = SDL_TRUE;
		break;
	case SDLK_ESCAPE:
		*done = SDL_TRUE;
		*gorde = -1;
		break;
	}
}

void moveright(int start, char* path) {
	int position = strlen(path);

	while (position >= start) {
		path[position + 1] = path[position];
		position--;
	}
}

void exportMenu(SDL_Renderer* renderer) {
	SDL_Texture* pathTitle;
	pathTitle = paintbackground(renderer);
	SDL_DestroyTexture(pathTitle);

	SDL_Surface* surf_save = NULL;
	SDL_Surface* surf_exit = NULL;
	loadIMG(&surf_save, "images/save.png");
	loadIMG(&surf_exit, "images/exit.png");

	if (surf_save && surf_exit) {
		SDL_Texture* Save = SDL_CreateTextureFromSurface(renderer, surf_save);
		SDL_Texture* Exit = SDL_CreateTextureFromSurface(renderer, surf_exit);
		SDL_FreeSurface(surf_save);
		SDL_FreeSurface(surf_exit);
		SDL_Rect Save_rect = { WIDTH / 2 - 96 / 2, 347 + 100, 96, 26 };
		SDL_Rect Exit_rect = { WIDTH / 2 - 96 / 2, 384 + 100, 96, 26 };

		SDL_RenderCopy(renderer, Save, NULL, &Save_rect);
		SDL_RenderCopy(renderer, Exit, NULL, &Exit_rect);
		SDL_RenderPresent(renderer);
	}
}

SDL_Texture* paintbackground(SDL_Renderer* renderer) {
	static SDL_Texture* back = NULL;
	if(!back)loadTexture(&back, renderer, "images/ExportBack.png");

	TTF_Font* DogicaBold = TTF_OpenFont("dogicapixelbold.ttf", 22);
	SDL_Color Black = { 0, 0, 0, 255 };
	int w, h;
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(DogicaBold, "Sartu direktorioa eta izena:", Black);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_QueryTexture(Message, NULL, NULL, &w, &h);
	SDL_Rect Message_rect = { WIDTH / 2 - w / 2, 257 - h + 50, w, h };
	SDL_Rect back_rect = { 0, 0, WIDTH, HEIGHT + MENU_HEIGHT };
	SDL_RenderCopy(renderer, back, NULL, &back_rect);
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(renderer, Message_rect.x, Message_rect.y + h + 2, Message_rect.x + w, Message_rect.y + h + 2);
	SDL_RenderDrawLine(renderer, Message_rect.x, Message_rect.y + h + 3, Message_rect.x + w, Message_rect.y + h + 3);
	SDL_RenderPresent(renderer);

	SDL_FreeSurface(surfaceMessage);
	TTF_CloseFont(DogicaBold);
	return Message;
}

SDL_Texture* showpath(char* path, SDL_Renderer* renderer, int position_path, SDL_Texture* Message) {
	char tmp[128];
	strcpy(tmp, path);

	TTF_Font* DogicaPixel = TTF_OpenFont("dogicapixel.ttf", 18);
	SDL_Color Black = { 0, 0, 0, 255 };

	int w, h, i = 0;
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(DogicaPixel, tmp, Black);
	Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_QueryTexture(Message, NULL, NULL, &w, &h);
	while (w > WIDTH - 404) {
		if (position_path > i) {
			SDL_FreeSurface(surfaceMessage);
			surfaceMessage = TTF_RenderText_Solid(DogicaPixel, tmp + i + 1, Black);
			i++;
		}
		else {
			tmp[strlen(tmp) - 1] = '\0';
			SDL_FreeSurface(surfaceMessage);
			surfaceMessage = TTF_RenderText_Solid(DogicaPixel, tmp + i, Black);
		}
		SDL_DestroyTexture(Message);
		Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_QueryTexture(Message, NULL, NULL, &w, &h);
	}

	int ptrPos = obtainPtrPosition(renderer, DogicaPixel, position_path, i, path);
	renderpath(renderer, Message, ptrPos, h, w);

	TTF_CloseFont(DogicaPixel);
	SDL_FreeSurface(surfaceMessage);

	return Message;
}

int formatuegokia(char* path) {
	if (strcmp(path + strlen(path) - 4, ".png") == 0) {
		return 1;
	}
	else return 0;
}

void renderpath (SDL_Renderer* renderer, SDL_Texture* Message, int ptrPos, int h, int w) {
	SDL_Rect Message_rect = { 202, 276 + h/2 + 90, w, h };
	SDL_Rect rect = { 200, 277 + 90, WIDTH - 400, 30 };
	SDL_Rect rect2 = { 198, 275 + 90, WIDTH - 400 + 4, 30 + 4 };

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect2);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, ptrPos, 279 + 90, ptrPos, 305 + 90);
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_RenderPresent(renderer);
}

int obtainPtrPosition(SDL_Renderer* renderer, TTF_Font* Verdana, int position_path, int i, char* path) {
	int ptrPos = 202, wTMP, h;
	char tmp2[128];
	SDL_Color Black = { 0,0,0,255 };
	strcpy(tmp2, path + i);
	tmp2[position_path - i] = '\0';
	if (strlen(tmp2) != 0) {
		SDL_Surface* surfaceTMP = TTF_RenderText_Solid(Verdana, tmp2, Black);
		SDL_Texture* MessageTMP = SDL_CreateTextureFromSurface(renderer, surfaceTMP);
		SDL_QueryTexture(MessageTMP, NULL, NULL, &wTMP, &h);
		SDL_FreeSurface(surfaceTMP);
		SDL_DestroyTexture(MessageTMP);
		ptrPos = wTMP + 201;
	}
	return ptrPos;
}

/*
Irudi bat inportatzeko pantailara arrastratuaz.
[1] Ebentoak begiratu ESC tekla sakatuaz irten edo fitxategia arrastratu arte.
[2] Fitxategiaren helbidea lortutakoan surface batera bihurtu eta beharrezko memoriak askatu.
*/
int importMap(SDL_Surface** surface, SDL_Renderer* renderer) {
	static SDL_Texture* importTex = NULL;
	if (!importTex) loadTexture(&importTex, renderer, "images/import_editor.png");
	SDL_Rect btn = { 0, 0, WIDTH, HEIGHT + MENU_HEIGHT };
	SDL_Rect btn_imp = { 106, 3, 96, 26 };
	SDL_RenderCopy(renderer, importTex, NULL, &btn);
	SDL_RenderPresent(renderer);
    char fileDir[256];
	int loaded = 0, click = 0;
    SDL_Event e;
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    while (!loaded)
        while (SDL_PollEvent(&e)) {
            eventHandler(e);
			switch (e.type) {
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					return 0;
				}
			case SDL_QUIT:
				return -1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (checkButton(btn_imp)) {
					click = 1;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (click == 1) {
					return 0;
				}
				break;
			case SDL_DROPFILE:
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

int importMap_menu(SDL_Surface** surface, SDL_Renderer* renderer) {
	static SDL_Texture* importTex = NULL;
	if (!importTex) loadTexture(&importTex, renderer, "images/import_menu.png");
	SDL_Rect btn = { (WIDTH - 741)/2, 340, 741, 425 };
	SDL_Rect btn_back = { 0,0,WIDTH,HEIGHT + MENU_HEIGHT };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
	SDL_RenderFillRect(renderer, &btn_back);
	SDL_RenderCopy(renderer, importTex, NULL, &btn);
	SDL_RenderPresent(renderer);
    char fileDir[256];
	int loaded = 0, click = 0;
    SDL_Event e;
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    while (!loaded)
        while (SDL_PollEvent(&e)) {
            eventHandler(e);
			switch (e.type) {
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					return 0;
				}
			case SDL_QUIT:
				return -1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!checkButton(btn)) {
					click = 1;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (click == 1) {
					return 0;
				}
				break;
			case SDL_DROPFILE:
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
