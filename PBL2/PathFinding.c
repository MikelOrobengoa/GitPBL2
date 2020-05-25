#include "PathFinding.h"

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
            nodes[j][j].block = 0;
            nodes[i][j].cost = 0;
            nodes[i][j].neighboursCount = 0;
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
            if (i + 1 <= X_TILES && j - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i +1 ][j - 1];
                    nodes[i][j].neighboursCount++;
            }
            if (i - 1 >= 0) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j];
                    nodes[i][j].neighboursCount++;
            }
            if (i + 1 <= X_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i + 1][j];
                nodes[i][j].neighboursCount++;
            }
            if (i - 1 >= 0 && j + 1 <= Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i - 1][j + 1];
                nodes[i][j].neighboursCount++;
            }
            if (j + 1 <= Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i][j + 1];
                nodes[i][j].neighboursCount++;
            }
            if (i + 1 <= X_TILES && j + 1 <= Y_TILES) {
                nodes[i][j].neighbours[nodes[i][j].neighboursCount] = &nodes[i + 1][j + 1];
                nodes[i][j].neighboursCount++;
            }
        }
    }
}

double calculateHValue(int ogX, int ogY, int destX, int destY) {
    getTilePos(&ogX, &ogY);
    getTilePos(&destX, &destY);
    return ((double) sqrt(((double) ogX / TILESIZE - destX / TILESIZE) * ((double) ogX / TILESIZE - destX / TILESIZE)
        + ((double) ogY / TILESIZE - destY / TILESIZE) * ((double) ogY / TILESIZE - destY / TILESIZE)));
}

int aStar() {
    initNodes();
    while (openKop > 0) {
        printf("openKop: %d\n", openKop);
        int lowestIndex = openKop - 1;
        node* current = open[lowestIndex];

        for (int i = openKop - 2; i >= 0; i--) {
            if (open[i]->f < open[lowestIndex]->f) lowestIndex = i;
        }
        current = open[lowestIndex];
        printf("current: (%d, %d)\n", current->x, current->y);

        removeNode(current, &openKop, open);
        closed[closedKop] = current;
        closedKop++;

        if (current->x == endNode->x && current->y == endNode->y) {
            retracePath(startNode, endNode);
            return 1;
        }

        for (int i = current->neighboursCount - 1; i >= 0; i--) {
            node* neighbour = current->neighbours[i];
            if (neighbour->block || containsNode(neighbour, closedKop, closed)) continue;
            else {
                double newNeighbourCost = current->f + calculateHValue(current->x, current->y, neighbour->x, neighbour->y);
                if (newNeighbourCost < neighbour->g || !containsNode(neighbour, openKop, open)) {
                    neighbour->g = newNeighbourCost;
                    neighbour->h = calculateHValue(neighbour->x, neighbour->y, endNode->x, endNode->y);
                    neighbour->parent = current;

                    if (!containsNode(neighbour, openKop, open)) {
                        open[openKop] = neighbour;
                        openKop++;
                    }
                }
            }
        }
    }

    return 0;
}

int containsNode(node* element, int kop, node** list) {
    int found = 0, i = 0;
    while (!found && i < kop){
        if (element->x == list[i]->x && element->y == list[i]->y) found = 1;
            i++;
    }
    return found;
}

void removeNode(node* element, int* kop, node** list) {
    int i = 0;
    while (element->x != list[i]->x && element->y != list[i]->y && i < *kop) i++;
    for (i; i < *kop - 1; i++){
        list[i] = list[i + 1];
    }
    *kop = *kop - 1;
}

void retracePath(node* start, node* end) {
    int i = 0;
    node* current = end;
    while (current->x != start->x || current->y != start->y) {
        //path = (node*)malloc(sizeof(node*) * i);
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
        printf("%d, (%d, %d)\n", i, path[i]->x, path[i]->y);

    }

}