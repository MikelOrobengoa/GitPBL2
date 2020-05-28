#include "PathFinding.h"

/*
Editorean hautatutako kolorea
*/
SDL_Color color = { 0, 0, 0 };

/*
Editorearen funtzio nagusia.
[1] Aurretik exportaturik gabe badago, eta gordetzeko botoia klikatzean irudia exportatu.
[2] Sagua menutik behera badago, 'canvas' barruan, klik egitean lauza margotu.
[3] Sagua menuan badago, bertako botoi desberdinak klikatu diren begiratu eta beharrezko funtzioak deitu edo kolorea aldatu
*/
int editor(SDL_Surface** surface, SDL_Renderer* renderer, int* clientState) {
    static int costColor = 4, button = -1;
    int changed = 0;
    SDL_Texture* menuBarTexture = NULL;
    SDL_Rect menuBar = { 0, 0, WIDTH, MENU_HEIGHT };
    SDL_Surface* menuBarSurface = NULL;
    loadEditorMenu(&menuBarSurface);
    if (menuBarSurface) menuBarTexture = SDL_CreateTextureFromSurface(renderer, menuBarSurface);
    SDL_FreeSurface(menuBarSurface);
    //Color palette
    
    SDL_Color
        red = { 255, 0, 0 },
        green = { 0, 255, 0 },
        blue = { 0, 0, 255 },
        magenta = { 255, 0, 255 },
        costs[] = {
            {.r = 255, .g = 255, .b = 255 },
            {.r = 153, .g = 153, .b = 153 },
            {.r = 102, .g = 102, .b = 102 },
            {.r = 51, .g = 51, .b = 51 },
            {.r = 0, .g = 0, .b = 0 }
        };
    int x, y;
    SDL_GetMouseState(&x, &y);
    if (y >= MENU_HEIGHT) { //Canvas
        if (MOUSE_CLICK) {
            paintTile(*surface, color);
            updateMap(color);
            changed = 1;
        }
        else {
            if (button != -1) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
            }
                button = -1;
        }
    }
    else { //Menu
        SDL_Rect
            btn_new = { 6, 3, 96, 26 },
            btn_import = { 106, 3, 96, 26 },
            btn_export = { 206, 3, 96, 26 },
            btn_red = { 341, 3, 28, 26 },
            btn_green = { 373, 3, 28, 26 },
            btn_blue = { 405, 3, 28, 26 },
            btn_minus = { 468, 3, 28, 26 },
            btn_cost = { 500, 3, 28, 26 },
            btn_plus = { 532, 3, 28, 26 },
            btn_sim = { 600, 3, 96, 26 },
            btn_help = { 734, 3, 28, 26 };
        if (MOUSE_CLICK) {
            if (checkButton(btn_new)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if(button == 0) drawIMG(renderer, "images/new_click.png", btn_new);
                button = 0;
            }
            else if (checkButton(btn_import)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 1)drawIMG(renderer, "images/import_click.png", btn_import);
                button = 1;
            }
            else if (checkButton(btn_export)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 2)drawIMG(renderer, "images/export_click.png", btn_export);
                button = 2;
            }
            else if (checkButton(btn_red)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 3)drawIMG(renderer, "images/red_click.png", btn_red);
                button = 3;
            }
            else if (checkButton(btn_green)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 4)drawIMG(renderer, "images/green_click.png", btn_green);
                button = 4;
            }
            else if (checkButton(btn_blue)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 5)drawIMG(renderer, "images/blue_click.png", btn_blue);
                button = 5;
            }
            else if (checkButton(btn_minus)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 6)drawIMG(renderer, "images/minus_click.png", btn_minus);
                button = 6;
            }
            else if (checkButton(btn_cost)) {
                // drawIMG(renderer, , btn_cost);
                button = 7;
            }
            else if (checkButton(btn_plus)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 8)drawIMG(renderer, "images/plus_click.png", btn_plus);
                button = 8;
            }
            else if (checkButton(btn_sim)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if (button == 9)drawIMG(renderer, "images/simulate_click.png", btn_sim);
                button = 9;
            }
            else if (checkButton(btn_help)) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                if(button == 10)drawIMG(renderer, "images/help_click.png", btn_help);
                button = 10;
            }       
        }
        else {
            if (button == 0) {
                SDL_Surface* s;
                s = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, NULL, NULL, NULL, NULL);
                if (!s) printf("Errorea fitxategi berria sortzean.\n");
                else {
                    SDL_FillRect(s, NULL, SDL_MapRGB((s)->format, 255, 255, 255));
                    SDL_FreeSurface(*surface);
                    *surface = s;
                    changed = 1;
                }
            }
            else if (button == 1) {
                SDL_Surface* s = NULL;
                importMap(&s);
                if (!s) printf("Errorea fitxategia inportatzean.\n");
                else {
                    SDL_FreeSurface(*surface);
                    *surface = s;
                    changed = 1;
                }
            }
            else if (button == 2) {
                switch (exportMap(*surface, renderer)) {
                case -1:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Ezin izan da exportatu fitxategia\n-Ez da aurkitu direktorioa", NULL);
                    break;
                case 0:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Ezin izan da exportatu fitxategia\n-Zihurtatu .png formatuan exportatzen dela", NULL);
                    break;
                case 1:
                    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "SAVE", "Exportatu da png fitxategia", NULL);
                    break;
                case 2:
                    changed = 2;
                    break;
                }
            }
            else if (button == 3) {
                color = red;
            }
            else if (button == 4) {
                color = green;
            }
            else if (button == 5) {
                color = blue;
            }
            else if (button == 6) {
                if (costColor > 0) {
                    costColor--;
                }
                printf("cost: %d\n", costColor);
                color = costs[costColor];
            }
            else if (button == 7) {
                color = costs[costColor];
            }
            else if (button == 8) {
                if (costColor < 4) {
                    costColor++;
                }
                printf("cost: %d\n", costColor);
                color = costs[costColor];
            }
            else if (button == 9) {
                *clientState = CLIENT_SIM;
            }
            else if (button == 10) {
                printf("HAHA NO HELP LUL\n");
            }
            SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
            button = -1;
        }   
    }

    return changed;
}

/*
Laukiak desberdintzeko kuadrikula. Pantailan zuzenean marrazten da, ez surfacean.
*/
void renderGrid(SDL_Renderer* renderer) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (int x = 0; x < X_TILES; x++) {
        SDL_RenderDrawLine(renderer, x * TILESIZE, MENU_HEIGHT, x * TILESIZE, HEIGHT + MENU_HEIGHT);
    }

    for (int y = 0; y < Y_TILES; y++) {
        SDL_RenderDrawLine(renderer, 0, y * TILESIZE + MENU_HEIGHT, WIDTH, y * TILESIZE + MENU_HEIGHT);
    }
}

/*
Saguak seinalatzen duen laukia kolore batez margotzea. Aldaketa hau zuzenean surface gainean egiten da.
*/
int paintTile(SDL_Surface* surface, SDL_Color color) {
    int posX, posY;
    SDL_GetMouseState(&posX, &posY);
    if (posY >= MENU_HEIGHT) {
        getTilePos(&posX, &posY);
        SDL_Rect tile = { posX, posY, TILESIZE, TILESIZE };
        SDL_FillRect(surface, &tile, SDL_MapRGB(surface->format, color.r, color.g, color.b));
    }
	return 0;
}

/*
Saguak erakusten duen laukia programan hautatutako kolorez gardentasunarekin irudikatu. Pantailan zuzenean marrazten da, ez surfacean.
*/
int renderTilePreview(SDL_Renderer* renderer) {
    int posX, posY, success = 0;
    SDL_GetMouseState(&posX, &posY);
    if (posY >= MENU_HEIGHT) {
        posY += MENU_HEIGHT;
        getTilePos(&posX, &posY);
        SDL_Rect tile = { posX, posY, TILESIZE, TILESIZE };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 172);
        SDL_RenderFillRect(renderer, &tile);
        success = 1;
    }
    return success;
}

/*
Posizio bat kuadrikulako lauki baten posiziora bihurtu.
*/
void getTilePos(int* x, int* y) {
    *x -= *x % TILESIZE;
    *y -= MENU_HEIGHT;
    *y -= *y % TILESIZE;
}

void renderEditor(SDL_Renderer* renderer, SDL_Texture* fileTexture) {
    static SDL_Texture* menuBarTexture = NULL;
    SDL_Rect menuBar = { 0, 0, WIDTH, MENU_HEIGHT };
    if (!menuBarTexture) {
        SDL_Surface* menuBarSurface = NULL;
        loadEditorMenu(&menuBarSurface);
        if (menuBarSurface) menuBarTexture = SDL_CreateTextureFromSurface(renderer, menuBarSurface);
        SDL_FreeSurface(menuBarSurface);
        SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
    }

    SDL_Rect screen = { 0, MENU_HEIGHT, WIDTH, HEIGHT};
    SDL_RenderCopy(renderer, fileTexture, NULL, &screen);
    renderGrid(renderer);
    renderTilePreview(renderer);
}

int loadEditorMenu(SDL_Surface** surface) {
    int loaded = 0;
    SDL_RWops* fp = SDL_RWFromFile("images/menuBar.png", "rb");
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

int loadIMG(SDL_Surface** surface, char* image_path) {
    int loaded = 0;
    SDL_RWops* fp = SDL_RWFromFile(image_path, "rb");
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
        SDL_Rect Save_rect = { WIDTH / 2 - 96 / 2, 270, 96, 26 };
        SDL_Rect Exit_rect = { WIDTH / 2 - 96 / 2, 307, 96, 26 };

        SDL_RenderCopy(renderer, Save, NULL, &Save_rect);
        SDL_RenderCopy(renderer, Exit, NULL, &Exit_rect);
        SDL_RenderPresent(renderer);
    }
}

SDL_Texture* paintbackground(SDL_Renderer* renderer) {
    SDL_Rect background = { 0, 0, 768 + MENU_HEIGHT, 768 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_RenderFillRect(renderer, &background);

    TTF_Init();
    TTF_Font* Verdana = TTF_OpenFont("verdanab.ttf", 24);
    SDL_Color White = { 255, 255, 255, 255 };
    int w, h;
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Verdana, "Sartu direktorioa eta izena:", White);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_QueryTexture(Message, NULL, NULL, &w, &h);
    SDL_Rect Message_rect = { 202, 180 - h, w, h };
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    SDL_RenderPresent(renderer);
    TTF_Quit();

    SDL_FreeSurface(surfaceMessage);
    TTF_CloseFont(Verdana);
    return Message;
}

void drawIMG(SDL_Renderer* renderer, char* path, SDL_Rect btn) {
    SDL_Surface* surf = NULL;
    loadIMG(&surf, path);
    if (surf) {
        SDL_Texture* Tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
        SDL_RenderCopy(renderer, Tex, NULL, &btn);
        SDL_RenderPresent(renderer);
    }
}