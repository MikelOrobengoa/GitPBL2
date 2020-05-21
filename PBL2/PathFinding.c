#include "PathFinding.h"

/*
Algoritmoak erabiliko dituen nodoen (laukien) zerrenda .
*/
node nodes[576];
/*
Bidearen hasiera eta bukaerako nodoak zerrendako zein posiziotan dauden.
*/
int startIndex = 0, endIndex = 0;

/*
Surface batean, lauki bakoitzaren lehen pixeleko kolorea begiratu eta horren arabera nodoari beharrezko balioa ezarri.
*/
int loadMap(SDL_Surface* surface) {
    int success = 1;
    for (int i = 0; i < 576; i++) {
        nodes[i].block = 0;
        nodes[i].cost = 0;
        nodes[i].end = 0;
        nodes[i].start = 0;
    }

	for (int y = 0; y < 24; y++) {
		for (int x = 0; x < 24; x++) {
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
        nodes[y * 24 + x].start = 1;
        endIndex = y * 24 + x;
    }
    else if (color.r == 0 && color.g == 255 && color.b == 0) {
        nodes[y * 24 + x].end = 1;
        startIndex = y * 24 + x;
    }
    else if (color.r == 153 && color.g == 153 && color.b == 153) nodes[y * 24 + x].cost = 1;
    else if (color.r == 102 && color.g == 102 && color.b == 102) nodes[y * 24 + x].cost = 2;
    else if (color.r == 51 && color.g == 51 && color.b == 51) nodes[y * 24 + x].cost = 3;
    else if (color.r == 0 && color.g == 0 && color.b == 0) nodes[y * 24 + x].cost = 4;
    else if (color.r == 255 && color.g == 0 && color.b == 0) nodes[y * 24 + x].block = 1;
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