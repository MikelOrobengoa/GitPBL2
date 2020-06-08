#include "General.h"
#include "PathFinding.h"
#include "Editor.h"
#include <math.h>

/*
Algoritmoak erabiliko dituen nodoen (laukien) zerrenda edo matrizeak.
*/
node nodes[X_TILES][Y_TILES];
node** open;
node** closed;
node** path;
int openKop, closedKop = 0, pathKop = 0;

/*
Bidearen hasiera eta bukaerako nodoak zerrendako zein posiziotan dauden.
*/
node* startNode = NULL, * endNode = NULL;

/*
Surface batean, lauki bakoitzaren lehen pixeleko kolorea begiratu eta horren arabera nodoari beharrezko balioa ezarri.
*/
int loadMap(SDL_Surface* surface) {
    int success = 1;
	for (int y = 0; y < Y_TILES; y++) {
		for (int x = 0; x < X_TILES; x++) {
            success = checkColor(surface, getPixelColor(surface, x, y), x, y);
            if (!success) break;
		}
        if (!success) break;
	}
    return success;
}

/*
Kolorearen arabera (x, y) posizioko nodoari beharrezko ezaugarriak ezarri.
*/
int checkColor(SDL_Surface* surface, SDL_Color color, int x, int y) {
    int success = 1;

    nodes[x][y].block = 0;
    nodes[x][y].cost = 0;
    nodes[x][y].x = x;
    nodes[x][y].y = y;
    nodes[x][y].id = x + y * Y_TILES;

    if (color.r == 0 && color.g == 0 && color.b == 255) {
        if (startNode != &nodes[x][y]) {
            if (startNode) {
                SDL_Rect tile = { startNode->x * TILESIZE, startNode->y * TILESIZE, TILESIZE, TILESIZE };
                SDL_FillRect(surface, &tile, SDL_MapRGB(surface->format, 255, 255, 255));
            }
            startNode = &nodes[x][y];
            openKop = 1;
            if (!memoriaKudeatu(&open, openKop)) printf("Errorea open malloc\n");
            open[0] = startNode;
        }
    }
    else if (color.r == 0 && color.g == 255 && color.b == 0) {
        if (endNode != &nodes[x][y]) {
            if (endNode) {
                SDL_Rect tile = { endNode->x * TILESIZE, endNode->y * TILESIZE, TILESIZE, TILESIZE };
                SDL_FillRect(surface, &tile, SDL_MapRGB(surface->format, 255, 255, 255));
            }
            endNode = &nodes[x][y];
        }
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

void updateMap(SDL_Surface* surface, SDL_Color color) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    getTilePos(&x, &y);
    checkColor(surface, color, x / TILESIZE, y / TILESIZE);
}

void initNodes(int diags) {
    endNode = NULL;
    startNode = NULL;
    for (int j = 0; j < Y_TILES; j++) {
        for (int i = 0; i < X_TILES; i++) {
            nodes[i][j].x = i;
            nodes[i][j].y = j;
            nodes[i][j].neighboursCount = 0;
            
            if (diags && i - 1 >= 0 && j - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j - 1];
                    nodes[i][j].neighboursCount++;
            }
            if (j - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i][j - 1];
                    nodes[i][j].neighboursCount++;
            }
            if (diags && i + 1 < X_TILES && j - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i +1 ][j - 1];
                    nodes[i][j].neighboursCount++;
            }
            if (i - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j];
                    nodes[i][j].neighboursCount++;
            }
            if (i + 1 < X_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i + 1][j];
                nodes[i][j].neighboursCount++;
            }
            if (diags && i - 1 >= 0 && j + 1 < Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j + 1];
                nodes[i][j].neighboursCount++;
            }
            if (j + 1 < Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i][j + 1];
                nodes[i][j].neighboursCount++;
            }
            if (diags && i + 1 < X_TILES && j + 1 < Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i + 1][j + 1];
                nodes[i][j].neighboursCount++;
            }
        }
    }
}

double calculateHValue(int ogX, int ogY, int destX, int destY, int diags) {
    if(diags) return sqrt(((double)ogX - destX) * ((double)ogX - destX) + ((double)ogY - destY) * ((double)ogY - destY));
    else {
        double d1, d2;
        d1 = abs(ogX - destX);
        d2 = abs(ogY - destY);
        if (d1 > d2) return d1;
        return d2;
    }
}

int findPath(SDL_Renderer* renderer, int alg, int diags) { 
    if (!endNode) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Errorea simulazioa kargatzean.\nEz dago bukaerako punturik.", NULL);
        return 0;
    }
    if (!startNode) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR", "Errorea simulazioa kargatzean.\nEz dago hasierako punturik.", NULL);
        return 0;
    }
    openKop = 1;
    if (!memoriaKudeatu(&open, openKop)) printf("Errorea open malloc\n");
    open[0] = startNode;

    closedKop = 0;
    while (openKop > 0) {
        int lowestIndex = 0;

        for (int i = 0; i < openKop; i++) {
            if (open[i]->f < open[lowestIndex]->f) lowestIndex = i;
        }
        node* current = open[lowestIndex];

        SDL_Rect r = { current->x * TILESIZE, current->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
        SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255);
        SDL_RenderFillRect(renderer, &r);

        if (!memoriaKudeatu(&closed, closedKop + 1)) printf("Errorea closed malloc\n");
        closed[closedKop] = current;
        closedKop++;
        removeNode(current, &openKop, open);
        if (!memoriaKudeatu(&open, openKop)) printf("Errorea open malloc\n");

        if (current->id == endNode->id) {
            openKop = 0;
            closedKop = 0;
            free(open);
            open = NULL;
            free(closed);
            closed = NULL;
            retracePath(startNode, endNode);
            return 1;
        }

        for (int i = 0; i < current->neighboursCount; i++) {
            node* neighbour = current->neighbours[i];
            if (neighbour->block || containsNode(neighbour, closedKop, closed))
                continue;
            else {
                double newNeighbourCost = current->g + neighbour->cost + calculateHValue(current->x, current->y, neighbour->x, neighbour->y, diags);
                if (newNeighbourCost < neighbour->g ||!containsNode(neighbour, openKop, open)) {
                    neighbour->g = newNeighbourCost;
                    if (alg == 1) {
                        neighbour->h = calculateHValue(neighbour->x, neighbour->y, endNode->x, endNode->y, diags);
                        neighbour->f = neighbour->g + neighbour->h;
                    }
                    else neighbour->f = neighbour->g;
                    neighbour->parent = current;

                    if (!containsNode(neighbour, openKop, open)) {
                        if (!memoriaKudeatu(&open, openKop + 1)) printf("Errorea open malloc\n");
                        open[openKop] = neighbour;
                        openKop++;
                    }
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < openKop; i++) {
            SDL_Rect r = { open[i]->x * TILESIZE, open[i]->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
            SDL_SetRenderDrawColor(renderer, 128, 255, 128, 255);
            SDL_RenderFillRect(renderer, &r);
        }
        for (int i = 0; i < closedKop; i++) {
            SDL_Rect r = { closed[i]->x * TILESIZE, closed[i]->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
            SDL_SetRenderDrawColor(renderer, 160, 255, 160, 80);
            SDL_RenderFillRect(renderer, &r);
        }
        for (int i = 0; i < closedKop; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 160, 160, 255);
            if(closed[i]->parent)SDL_RenderDrawLine(renderer, closed[i]->x * TILESIZE + TILESIZE / 2, closed[i]->y * TILESIZE + TILESIZE + TILESIZE / 2, closed[i]->parent->x * TILESIZE + TILESIZE / 2, closed[i]->parent->y * TILESIZE + TILESIZE + TILESIZE / 2);
        }
        for (int i = 0; i < openKop; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 160, 160, 255);
           if(open[i]->parent) SDL_RenderDrawLine(renderer, open[i]->x * TILESIZE + TILESIZE / 2, open[i]->y * TILESIZE + TILESIZE + TILESIZE / 2, open[i]->parent->x * TILESIZE + TILESIZE / 2, open[i]->parent->y * TILESIZE + TILESIZE + TILESIZE / 2);
        }
        SDL_RenderPresent(renderer);
    }
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "EZINEZKOA", "Ez dago bide posiblerik.", NULL);
    MOUSE_CLICK = 0;
    return 0;
}

int containsNode(node* element, int kop, node** list) {
    int found = 0, i = 0;
    while (!found && i < kop){
        if (element->id == list[i]->id) found = 1;
        i++;
    }
    return found;
}

void removeNode(node* element, int* kop, node** list) {
    int i = 0;
    while (i < *kop && element->id != list[i]->id) i++;
    for (i; i < *kop - 1; i++) {
        list[i] = list[i + 1];
    }
    *kop -= 1;
}

void retracePath(node* start, node* end) {
    int i = 0;
    node* current = end;
    while (current->id != start->id) {
        if (!memoriaKudeatu(&path, i + 1)) printf("Errorea path malloc\n");
        *(path + i) = current;
        i++;
        current = current->parent;
    }
    *(path + i) = start;
    i++;
    pathKop = i;
}

void printfPath(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    for (int i = 0; i < pathKop - 1; i++) {
        for (int j = -1; j < 2; j++) {
            for (int k = -1; k < 2; k++) {
                SDL_RenderDrawLine(renderer,
                    path[i]->x * TILESIZE + j + TILESIZE / 2,
                    path[i]->y * TILESIZE + TILESIZE + k + TILESIZE / 2,
                    path[i + 1]->x * TILESIZE + j + TILESIZE / 2,
                    path[i + 1]->y * TILESIZE + TILESIZE + k + TILESIZE / 2);
            }
        }
    }
}

node** getCurrentPath(int* kop) {
    *kop = pathKop;
    return path;
}

int memoriaKudeatu(node*** ptr, int kop) {
    int success = 0;
    node** ptrTmp = *ptr, ** ptrTmp2 = NULL;
    if (*ptr == NULL) {
        *ptr = (node**)malloc(sizeof(node*) * kop);
        success = 1;
    }
    else {
        if (kop == 0) {
            free(*ptr);
            *ptr = NULL;
        }
        else {
            ptrTmp2 = (node**)realloc(ptrTmp, sizeof(node*) * kop);
            if (ptrTmp2) {
                *ptr = ptrTmp2;
                success = 1;
            }
        }
    }
    return success;
}