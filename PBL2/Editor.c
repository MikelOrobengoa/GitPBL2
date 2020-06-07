#include "General.h"
#include "Editor.h"
#include "FileManager.h"
#include "PathFinding.h"
#include "Menu.h"

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
    int changed = 0, sim_state = 0;
    static SDL_Texture* menuBarTexture = NULL, *red_click = NULL, *blue_click = NULL, *green_click = NULL, *c0 = NULL, *c0_click = NULL, * c1 = NULL, * c1_click = NULL, * c2 = NULL, * c2_click = NULL, * c3 = NULL, * c3_click = NULL, * c4 = NULL, * c4_click = NULL, *New = NULL, *Export = NULL, *Import = NULL, *minus = NULL, * plus = NULL, *help = NULL, *A = NULL, *A2 = NULL, *Dijstra = NULL, *Dijstra2 = NULL, *diagonal = NULL, *diagonal2 = NULL, *straight = NULL, *straight2 = NULL, *play = NULL, *play2 = NULL;
    if (!menuBarTexture) {
        loadTexture(&menuBarTexture, renderer, "images/menuBar.png");
        loadTexture(&red_click, renderer, "images/red_click.png");
        loadTexture(&blue_click, renderer, "images/blue_click.png");
        loadTexture(&green_click, renderer, "images/green_click.png");
        loadTexture(&New, renderer, "images/new_click.png");
        loadTexture(&Export, renderer, "images/export_click.png");
        loadTexture(&Import, renderer, "images/import_click.png");
        loadTexture(&help, renderer, "images/help_click.png");
        loadTexture(&minus, renderer, "images/minus_click.png");
        loadTexture(&plus, renderer, "images/plus_click.png");
        loadTexture(&A, renderer, "images/a1.png");
        loadTexture(&A2, renderer, "images/a2.png");
        loadTexture(&Dijstra, renderer, "images/dijkstra1.png");
        loadTexture(&Dijstra2, renderer, "images/dijkstra2.png");
        loadTexture(&straight, renderer, "images/straight1.png");
        loadTexture(&straight2, renderer, "images/straight2.png");
        loadTexture(&diagonal, renderer, "images/diagonal1.png");
        loadTexture(&diagonal2, renderer, "images/diagonal2.png");
        loadTexture(&play, renderer, "images/play1.png");
        loadTexture(&play2, renderer, "images/play2.png");
        loadCost(renderer, &c0, &c0_click, &c1, &c1_click, &c2, &c2_click, &c3, &c3_click, &c4, &c4_click);
    }
    SDL_Rect
        menuBar = { 0, 0, WIDTH, MENU_HEIGHT },
        btn_new = { 6, 3, 96, 26 },
        btn_import = { 106, 3, 96, 26 },
        btn_export = { 206, 3, 96, 26 },
        btn_red = { 341, 3, 28, 26 },
        btn_green = { 373, 3, 28, 26 },
        btn_blue = { 405, 3, 28, 26 },
        btn_minus = { 468, 3, 28, 26 },
        btn_cost = { 500, 3, 28, 26 },
        btn_plus = { 532, 3, 28, 26 },
        btn_alg = { 569, 3, 62, 26 },
        btn_diag = { 633, 3, 62, 26 },
        btn_play = { 697, 3, 28, 26 },
        btn_help = { 734, 3, 28, 26 };
    
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
                if (!sim_alg)SDL_RenderCopy(renderer, Dijstra, NULL, &btn_alg);
                else SDL_RenderCopy(renderer, A, NULL, &btn_alg);
                if (sim_diags)SDL_RenderCopy(renderer, diagonal, NULL, &btn_diag);
                else SDL_RenderCopy(renderer, straight, NULL, &btn_diag);
                SDL_RenderCopy(renderer, play, NULL, &btn_play);
            }
            button = -1;
        }
    }
    else { //Menu
        if (MOUSE_CLICK) {
            if (checkButton(btn_new)) {
                if (button != new) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == new && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, New, NULL, &btn_new);
                }
                button = new;
            }
            else if (checkButton(btn_import)) {
                if (button != import) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == import && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, Import, NULL, &btn_import);
                }
                button = import;
            }
            else if (checkButton(btn_export)) {
                if (button != export) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == export && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, Export, NULL, &btn_export);
                }
                button = export;
            }
            else if (checkButton(btn_red)) {
                if (button != Red) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Red && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, red_click, NULL, &btn_red);
                }
                button = Red;
            }
            else if (checkButton(btn_green)) {
                if (button != Green) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Green && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, green_click, NULL, &btn_green);
                }
                button = Green;
            }
            else if (checkButton(btn_blue)) {
                if (button != Blue) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Blue && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, blue_click, NULL, &btn_blue);
                }
                button = Blue;
            }
            else if (checkButton(btn_minus)) {
                if (button != Minus) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Minus && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, minus, NULL, &btn_minus);
                }
                button = Minus;
            }
            else if (checkButton(btn_cost)) {
                if (button != Cost) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Cost && button != button_aurrekoa) {
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
                button = Cost;
            }
            else if (checkButton(btn_plus)) {
                if (button != Plus) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Plus && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, plus, NULL, &btn_plus);
                }
                button = Plus;
            }
            else if (checkButton(btn_alg)) {
                if (button != alg) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == alg && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    if(!sim_alg)SDL_RenderCopy(renderer, Dijstra2, NULL, &btn_alg);
                    else SDL_RenderCopy(renderer, A2, NULL, &btn_alg);
                }
                    button = alg;
            }
            else if (checkButton(btn_diag)) {
                if (button != diag) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == diag && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    if(sim_diags)SDL_RenderCopy(renderer, diagonal2, NULL, &btn_diag);
                    else SDL_RenderCopy(renderer, straight2, NULL, &btn_diag);
                }
                    button = diag;
            }
            else if (checkButton(btn_play)) {
                if (button != Play) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Play && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, play2, NULL, &btn_play);
                }
                    button = Play;
            }
            else if (checkButton(btn_help)) {
                if (button != Help) {
                    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
                    button_aurrekoa = button;
                }
                else if (button == Help && button != button_aurrekoa) {
                    button_aurrekoa = button;
                    SDL_RenderCopy(renderer, help, NULL, &btn_help);
                }
                button = Help;
            }
            if (button != Cost && button != button_aurrekoa) {
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
            if (button != alg && button != button_aurrekoa) {
                if (!sim_alg)SDL_RenderCopy(renderer, Dijstra, NULL, &btn_alg);
                else SDL_RenderCopy(renderer, A, NULL, &btn_alg);
            }
            if (button != diag && button != button_aurrekoa) {
                if (sim_diags)SDL_RenderCopy(renderer, diagonal, NULL, &btn_diag);
                else SDL_RenderCopy(renderer, straight, NULL, &btn_diag);
            }
            if (button != Play && button != button_aurrekoa)SDL_RenderCopy(renderer, play, NULL, &btn_play);
            SDL_RenderPresent(renderer);
        }
        else {
            SDL_Surface* s = NULL;
            switch (button) {
            case new:
                s = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
                if (!s) printf("Errorea fitxategi berria sortzean.\n");
                else {
                    SDL_FillRect(s, NULL, SDL_MapRGB((s)->format, 255, 255, 255));
                    SDL_FreeSurface(*surface);
                    *surface = s;
                    changed = 1;
                }
                break;
            case import:
                if (importMap(&s, renderer) == -1) changed = 2;
                if (!s) printf("Errorea fitxategia inportatzean.\n");
                else {
                    SDL_FreeSurface(*surface);
                    *surface = s;
                    changed = 1;
                    loadMap(*surface);
                }
                break;
            case export:
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
                break;
            case Red:
                color = red;
                break;
            case Green:
                color = green;
                break;
            case Blue:
                color = blue;
                break;
            case Minus:
                if (costColor > 0) {
                    costColor--;
                }
                color = costs[costColor];
                break;
            case Cost:
                color = costs[costColor];
                break;
            case Plus:
                if (costColor < 4) {
                    costColor++;
                }
                color = costs[costColor];
                break;
            case alg:
                if (sim_alg) sim_alg = 0;
                else sim_alg = 1;
                break;
            case diag:
                if (sim_diags) sim_diags = 0;
                else sim_diags = 1;
                break;
            case Play:
                *clientState = CLIENT_SIM;
                break;
            case Help:
                if (!helpmenu(renderer)) changed = 2;
                break;
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
                if (!sim_alg)SDL_RenderCopy(renderer, Dijstra, NULL, &btn_alg);
                else SDL_RenderCopy(renderer, A, NULL, &btn_alg);
                if (sim_diags)SDL_RenderCopy(renderer, diagonal, NULL, &btn_diag);
                else SDL_RenderCopy(renderer, straight, NULL, &btn_diag);
                SDL_RenderCopy(renderer, play, NULL, &btn_play);
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