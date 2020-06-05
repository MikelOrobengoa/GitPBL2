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
    static int costColor = 4, button = -1, button_aurrekoa = -1;
    int changed = 0;
    SDL_Rect menuBar = { 0, 0, WIDTH, MENU_HEIGHT }, btn_cost = { 500, 3, 28, 26 };
    static SDL_Texture* menuBarTexture = NULL, *red_click = NULL, *blue_click = NULL, *green_click = NULL, *c0 = NULL, *c0_click = NULL, * c1 = NULL, * c1_click = NULL, * c2 = NULL, * c2_click = NULL, * c3 = NULL, * c3_click = NULL, * c4 = NULL, * c4_click = NULL, *New = NULL, *Export = NULL, *Import = NULL, *minus = NULL, * plus = NULL, *help = NULL, *simulate = NULL;
    if (!menuBarTexture) {
        loadTexture(&menuBarTexture, renderer, "images/menuBar.png");
        loadTexture(&red_click, renderer, "images/red_click.png");
        loadTexture(&blue_click, renderer, "images/blue_click.png");
        loadTexture(&green_click, renderer, "images/green_click.png");
        loadTexture(&New, renderer, "images/new_click.png");
        loadTexture(&Export, renderer, "images/export_click.png");
        loadTexture(&Import, renderer, "images/import_click.png");
        loadTexture(&help, renderer, "images/help_click.png");
        loadTexture(&simulate, renderer, "images/simulate_click.png");
        loadTexture(&minus, renderer, "images/minus_click.png");
        loadTexture(&plus, renderer, "images/plus_click.png");
        loadCost(renderer, &c0, &c0_click, &c1, &c1_click, &c2, &c2_click, &c3, &c3_click, &c4, &c4_click);
    }
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
            updateMap(*surface, color);
            changed = 1;
        }
        else {
            if (button == -1) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                switch (costColor) {
                case 0:
                    SDL_RenderCopy(renderer, c0, NULL, &btn_cost);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, c1, NULL, &btn_cost);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, c2, NULL, &btn_cost);
                    break;
                case 3:
                    SDL_RenderCopy(renderer, c3, NULL, &btn_cost);
                    break;
                case 4:
                    SDL_RenderCopy(renderer, c4, NULL, &btn_cost);
                    break;
                }
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
            btn_plus = { 532, 3, 28, 26 },
            btn_sim = { 600, 3, 96, 26 },
            btn_help = { 734, 3, 28, 26 };
        if (MOUSE_CLICK) {
            if (checkButton(btn_new)) {
                if (button != 0) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 0 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, New, NULL, &btn_new);
                }
                button = 0;
            }
            else if (checkButton(btn_import)) {
                if (button != 1) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 1 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, Import, NULL, &btn_import);
                }
                button = 1;
            }
            else if (checkButton(btn_export)) {
                if (button != 2) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 2 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, Export, NULL, &btn_export);
                }
                button = 2;
            }
            else if (checkButton(btn_red)) {
                if (button != 3) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 3 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, red_click, NULL, &btn_red);
                }
                button = 3;
            }
            else if (checkButton(btn_green)) {
                if (button != 4) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 4 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, green_click, NULL, &btn_green);
                }
                button = 4;
            }
            else if (checkButton(btn_blue)) {
                if (button != 5) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 5 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, blue_click, NULL, &btn_blue);
                }
                button = 5;
            }
            else if (checkButton(btn_minus)) {
                if (button != 6) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 6 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, minus, NULL, &btn_minus);
                }
                button = 6;
            }
            else if (checkButton(btn_cost)) {
                if (button != 7) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 7 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    switch (costColor) {
                    case 0:
                        SDL_RenderCopy(renderer, c0_click, NULL, &btn_cost);
                        break;
                    case 1:
                        SDL_RenderCopy(renderer, c1_click, NULL, &btn_cost);
                        break;
                    case 2:
                        SDL_RenderCopy(renderer, c2_click, NULL, &btn_cost);
                        break;
                    case 3:
                        SDL_RenderCopy(renderer, c3_click, NULL, &btn_cost);
                        break;
                    case 4:
                        SDL_RenderCopy(renderer, c4_click, NULL, &btn_cost);
                        break;
                    }
                }
                button = 7;
            }
            else if (checkButton(btn_plus)) {
                if (button != 8) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 8 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, plus, NULL, &btn_plus);
                }
                button = 8;
            }
            else if (checkButton(btn_sim)) {
                if (button != 9) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 9 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, simulate, NULL, &btn_sim);
                }
                    button = 9;
            }
            else if (checkButton(btn_help)) {
                if (button != 10) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == 10 && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, help, NULL, &btn_help);
                }
                button = 10;
            }
            if (button != 7 && button != button_aurrekoa) {
                switch (costColor) {
                case 0:
                    SDL_RenderCopy(renderer, c0, NULL, &btn_cost);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, c1, NULL, &btn_cost);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, c2, NULL, &btn_cost);
                    break;
                case 3:
                    SDL_RenderCopy(renderer, c3, NULL, &btn_cost);
                    break;
                case 4:
                    SDL_RenderCopy(renderer, c4, NULL, &btn_cost);
                    break;
                }
            }
            SDL_RenderPresent(renderer);
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
                    loadMap(*surface);
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
                if (!helpmenu(renderer)) changed = 2;
                }
            if (button != -1) {
                SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                switch (costColor) {
                case 0:
                    SDL_RenderCopy(renderer, c0, NULL, &btn_cost);
                    break;
                case 1:
                    SDL_RenderCopy(renderer, c1, NULL, &btn_cost);
                    break;
                case 2:
                    SDL_RenderCopy(renderer, c2, NULL, &btn_cost);
                    break;
                case 3:
                    SDL_RenderCopy(renderer, c3, NULL, &btn_cost);
                    break;
                case 4:
                    SDL_RenderCopy(renderer, c4, NULL, &btn_cost);
                    break;
                }
                button = -1;
            }
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
        posX /= TILESIZE;
        posY /= TILESIZE;
        checkColor(surface, color, posX, posY);
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
            if (*surface) SDL_FreeSurface(*surface);
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
            if (*surface) SDL_FreeSurface(*surface);
            *surface = s;
            loaded = 1;
        }
        SDL_RWclose(fp);
    }

    return loaded;
}

void loadTexture(SDL_Texture** Tex, SDL_Renderer* renderer, char* path) {
    SDL_Surface* surface = NULL;

    loadIMG(&surface, path);
    if (surface) {
        *Tex = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }
}

void loadCost(SDL_Renderer* renderer, SDL_Texture** c0, SDL_Texture** c0_click, SDL_Texture** c1, SDL_Texture** c1_click, SDL_Texture** c2, SDL_Texture** c2_click, SDL_Texture** c3, SDL_Texture** c3_click, SDL_Texture** c4, SDL_Texture** c4_click) {
    loadTexture(c0, renderer, "images/cost0.png");
    loadTexture(c1, renderer, "images/cost1.png");
    loadTexture(c2, renderer, "images/cost2.png");
    loadTexture(c3, renderer, "images/cost3.png");
    loadTexture(c4, renderer, "images/cost4.png");
    loadTexture(c0_click, renderer, "images/cost0_click.png");
    loadTexture(c1_click, renderer, "images/cost1_click.png");
    loadTexture(c2_click, renderer, "images/cost2_click.png");
    loadTexture(c3_click, renderer, "images/cost3_click.png");
    loadTexture(c4_click, renderer, "images/cost4_click.png");
}

int helpmenu(SDL_Renderer* renderer) {
    static SDL_Texture* help = NULL;
    if (!help) loadTexture(&help, renderer, "images/help.png");
    SDL_Rect rect = { 0, 0, WIDTH, HEIGHT + MENU_HEIGHT }, btn_help = { 734, 3, 28, 26 };
    int done = -1, click = 0;

    SDL_RenderCopy(renderer, help, NULL, &rect);
    SDL_RenderPresent(renderer);

    while (done == -1) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                done = 0;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (checkButton(btn_help)) {
                    click = 1;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (click == 1) {
                    done = 1;
                }
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    done = 1;
                }
                break;
            }
        }
    }
    return done;
}