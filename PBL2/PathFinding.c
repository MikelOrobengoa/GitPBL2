#include "PathFinding.h"

/*
Algoritmoak erabiliko dituen nodoen (laukien) zerrenda .
*/
node nodes[24][24];
/*
Bidearen hasiera eta bukaerako nodoak zerrendako zein posiziotan dauden.
*/
int startIndex[] = { -1, -1 }, endIndex[] = { -1, -1 };

/*
Surface batean, lauki bakoitzaren lehen pixeleko kolorea begiratu eta horren arabera nodoari beharrezko balioa ezarri.
*/
int loadMap(SDL_Surface* surface) {
    int success = 1;
    for (int i = 0; i < Y_TILES; i++) {
        for (int j = 0; X_TILES < 24; j++) {
            nodes[i][j].block = 0;
            nodes[i][j].cost = 0;
            nodes[i][j].end = 0;
            nodes[i][j].start = 0;
        }
    }

	for (int y = 0; y < Y_TILES; y++) {
		for (int x = 0; x < X_TILES; x++) {
            success = checkColor(getPixelColor(surface, x, y), x, y);
            if (!success) break;
		}
        if (!success) break;
	}
    return success;
}

/*
Kolorearen arabera (x, y) posizioko nodoari beharrezko ezaugarriak ezarri.
*/
int checkColor(SDL_Color color, int x, int y) {
    int success = 1;
    if (color.r == 0 && color.g == 0 && color.b == 255) {
        nodes[x][y].start = 1;
        endIndex[0] = x;
        endIndex[1] = y;
    }
    else if (color.r == 0 && color.g == 255 && color.b == 0) {
        nodes[x][y].end = 1;
        startIndex[0] = x;
        startIndex[1] = y;
    }
    else if (color.r == 153 && color.g == 153 && color.b == 153) nodes[x][y].cost = 1;
    else if (color.r == 102 && color.g == 102 && color.b == 102) nodes[x][y].cost = 2;
    else if (color.r == 51 && color.g == 51 && color.b == 51) nodes[x][y].cost = 3;
    else if (color.r == 0 && color.g == 0 && color.b == 0) nodes[x][y].cost = 4;
    else if (color.r == 255 && color.g == 0 && color.b == 0) nodes[x][y].block = 1;
    else if (color.r != 255 && color.g != 255 && color.b != 255) success = 0;

    return success;
}

/*
Surface baten (x, y) posiziodun pixelaren RGB kolorea atera.
*/
SDL_Color getPixelColor(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint32* pixel = (Uint8*)surface->pixels + y * 32 * surface->pitch + x * 32 * bpp;
    SDL_Color color;
    SDL_GetRGB(*pixel, surface->format, &color.r, &color.g, &color.b);
    return color;
}