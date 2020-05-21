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
int editor(SDL_Surface** surface) {
    static int isSaved, costColor = 4;
    int changed = 0;


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
            isSaved = 0;
            changed = 1;
        }
    }
    else { //Menu
        if (MOUSE_CLICK) {
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

            if (checkButton(btn_new)) {
                SDL_Surface* s;
                s = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, NULL, NULL, NULL, NULL);
                if (!s) printf("Errorea fitxategi berria sortzean.\n");
                else {
                    SDL_FillRect(s, NULL, SDL_MapRGB((s)->format, 255, 255, 255));
                    SDL_FreeSurface(*surface);
                    *surface = s;
                    isSaved = 0;
                    changed = 1;
                }
            }
            else if (checkButton(btn_import)) {
                SDL_Surface* s = NULL;
                importMap(&s);
                if (!s) printf("Errorea fitxategia inportatzean.\n");
                else {
                    SDL_FreeSurface(*surface);
                    *surface = s;
                    isSaved = 0;
                    changed = 1;
                }
            }
            else if (checkButton(btn_export) && !isSaved) {
                exportMap(*surface);
                isSaved = 1;
            }
            else if (checkButton(btn_red)) {
                color = red;
            }
            else if (checkButton(btn_green)) {
                color = green;
            }
            else if (checkButton(btn_blue)) {
                color = blue;
            }
            else if (checkButton(btn_minus)) {
                if (costColor > 0) {
                    costColor--;
                }
                printf("cost: %d\n", costColor);
                color = costs[costColor];
            }
            else if (checkButton(btn_cost)) {
                color = costs[costColor];
            }
            else if (checkButton(btn_plus)) {
                if (costColor < 4) {
                    costColor++;
                }
                printf("cost: %d\n", costColor);
                color = costs[costColor];
            }
            else if (checkButton(btn_sim)) {
                printf("HAHA NO SIM SIMP\n");
            }
            else if (checkButton(btn_help)) {
                printf("HAHA NO HELP LUL\n");
            }
            MOUSE_CLICK = 0;
            //Colors
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

void renderEditor(SDL_Renderer* renderer, SDL_Surface* fileTexture) {
    static SDL_Texture* menuBarTexture = NULL;
    if (!menuBarTexture) {
        SDL_Surface* menuBarSurface = NULL;
        loadEditorMenu(&menuBarSurface);
        if (menuBarSurface) menuBarTexture = SDL_CreateTextureFromSurface(renderer, menuBarSurface);
        SDL_FreeSurface(menuBarSurface);
    }

    SDL_Rect screen = { 0, MENU_HEIGHT, WIDTH, HEIGHT};
    SDL_Rect menuBar = { 0, 0, WIDTH, MENU_HEIGHT };
    SDL_RenderCopy(renderer, fileTexture, NULL, &screen);
    renderGrid(renderer);
    renderTilePreview(renderer);
    SDL_RenderCopy(renderer, menuBarTexture, NULL, &menuBar);
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