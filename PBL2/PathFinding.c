#include "PathFinding.h"
#include <math.h>
#include <windows.h>

/*
Algoritmoak erabiliko dituen nodoen (laukien) zerrenda edo matrizeak.
*/
node nodes[X_TILES][Y_TILES];
node* open[X_TILES * Y_TILES];
node* closed[X_TILES * Y_TILES];
node* path[100];
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

    nodes[x][y].block = 0;
    nodes[x][y].cost = 0;
    nodes[x][y].id = x + y * Y_TILES;

    if (color.r == 0 && color.g == 0 && color.b == 255) {
        startNode = &nodes[x][y];
        open[0] = startNode;
        openKop = 1;
    }
    else if (color.r == 0 && color.g == 255 && color.b == 0) {
        endNode = &nodes[x][y];
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

void updateMap(SDL_Color color) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    getTilePos(&x, &y);
    checkColor(color, x / TILESIZE, y / TILESIZE);
}

void initNodes() {
    for (int j = 0; j < Y_TILES; j++) {
        for (int i = 0; i < X_TILES; i++) {
            nodes[i][j].x = i;
            nodes[i][j].y = j;
            
            if (i - 1 >= 0 && j - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j - 1];
                    nodes[i][j].neighboursCount++;
            }
            if (j - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i][j - 1];
                    nodes[i][j].neighboursCount++;
            }
            if (i + 1 < X_TILES && j - 1 >= 0) {
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
            if (i - 1 >= 0 && j + 1 < Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j + 1];
                nodes[i][j].neighboursCount++;
            }
            if (j + 1 < Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i][j + 1];
                nodes[i][j].neighboursCount++;
            }
            if (i + 1 < X_TILES && j + 1 < Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i + 1][j + 1];
                nodes[i][j].neighboursCount++;
            }
        }
    }
}

double calculateHValue(int ogX, int ogY, int destX, int destY) {
    return sqrt(pow((double)ogX - destX, 2) + pow((double)ogY - destY, 2));
    //double d1, d2;
    //d1 = abs(ogX - destX);
    //d2 = abs(ogY - destY);
    //if (d1 > d2) return d1;
    //else return d2;

    //return ((double) sqrt(((double) ogX - destX) * ((double) ogX - destX)
    //    + ((double) ogY - destY) * ((double) ogY - destY)));
}

int aStar(SDL_Renderer* renderer) {
    /*for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderClear(renderer, NULL);
            SDL_Rect r = { nodes[i][j].x * TILESIZE,  nodes[i][j].y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
            SDL_RenderFillRect(renderer, &r);
            for (int n = 0; n < nodes[i][j].neighboursCount; n++) {
                SDL_Rect r = { nodes[i][j].neighbours[n]->x * TILESIZE, nodes[i][j].neighbours[n]->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
                SDL_SetRenderDrawColor(renderer, 196, 64, 64, 255);
                SDL_RenderFillRect(renderer, &r);
                SDL_RenderPresent(renderer);
            }
                Sleep(1000);
        }
    }*/
    
    SDL_RenderPresent(renderer);
    for (int i = 0; i < 24; i++) printf("%d", nodes[i][3].block);
    openKop = 1;
    open[0] = startNode;
    closedKop = 0;
    while (openKop > 0) {
        Sleep(10);
        int lowestIndex = 0;

        for (int i = 1; i < openKop; i++) {
            if (open[i]->f <= open[lowestIndex]->f && open[i]->h <= open[lowestIndex]->h) lowestIndex = i;
        }
        node* current = open[lowestIndex];
        //printf("current: (%d, %d)\n", current->x, current->y);

        closed[closedKop] = current;
        closedKop++;
        removeNode(current, &openKop, open);

        if (current->id == endNode->id) {
            //for (int i = 0; i < openKop; i++) printf("%d, %d, %d", open[i]->x, open[i]->y, open[i]->id);
            printf("creo que ya he terminado");
            retracePath(startNode, endNode);
            return 1;
        }

        for (int i = 0; i < current->neighboursCount; i++) {
            node* neighbour = current->neighbours[i];
            if (neighbour->block || containsNode(neighbour, closedKop, closed))
                continue;
            else {
                double newNeighbourCost = current->g + calculateHValue(current->x, current->y, neighbour->x, neighbour->y) + neighbour->cost;
                //double newNeighbourCost = current->g + neighbour->cost + 1;
                if (newNeighbourCost <= neighbour->g ||!containsNode(neighbour, openKop, open)) {
                    if (newNeighbourCost <= neighbour->g) printf("ha gorim");
                    neighbour->g = newNeighbourCost;
                    neighbour->h = calculateHValue(neighbour->x, neighbour->y, endNode->x, endNode->y);
                    neighbour->f = neighbour->g + neighbour->h;
                    neighbour->parent = current;

                    if (!containsNode(neighbour, openKop, open)) {
                        open[openKop] = neighbour;
                        openKop++;
                    }
                }
            }
        }

        for (int i = 0; i < openKop; i++) {
            SDL_Rect r = { open[i]->x * TILESIZE, open[i]->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 32);
            SDL_RenderFillRect(renderer, &r);
        }
        for (int i = 0; i < closedKop; i++) {
            SDL_Rect r = { closed[i]->x * TILESIZE, closed[i]->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
            SDL_SetRenderDrawColor(renderer, 196, 64, 64, 32);
            SDL_RenderFillRect(renderer, &r);
        }
        SDL_RenderPresent(renderer);
    }

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
    while (i < *kop && element->id == list[i]->id) i++;
    for (i = i - 1; i < *kop; i++) {
        list[i] = list[i + 1];
    }
    *kop -= 1;
}

void retracePath(node* start, node* end) {
    int i = 0;
    node* current = end;
    while (current->id != start->id) {
        *(path + i) = current;
        i++;
        current = current->parent;
    }
    pathKop = i;
}

void printfPath(SDL_Renderer* renderer) {
    for (int i = 0; i < pathKop; i++) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_Rect r = { path[i]->x * TILESIZE, path[i]->y * TILESIZE + TILESIZE, TILESIZE, TILESIZE };
        SDL_RenderFillRect(renderer, &r);
        printf("%d, (%d, %d) g: %f, h: %f f: %f\n", i, path[i]->x, path[i]->y, path[i]->g, path[i]->h, path[i]->f);

    }

}