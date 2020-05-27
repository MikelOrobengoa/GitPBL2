#ifndef PATHFINDING
#define PATHFINDING
#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

/*
'Canvas'-aren zabalera
*/
#define WIDTH 768
/*
'Canvas'-aren altuera
*/
#define HEIGHT 768
/*
Editoreko menu-barreren altuera
*/
#define MENU_HEIGHT 32
/*
Maparen zabalera laukitan
*/
#define X_TILES 24
/*
Maparen altuera laukitan
*/
#define Y_TILES 24
/*
Laukien tamaina
*/
#define TILESIZE 32

enum CLIENT_STATE { CLIENT_MAIN_MENU, CLIENT_EDITOR, CLIENT_SIM };

extern int KEYS[322];
extern int MOUSE_CLICK;

/*
Biderik motzena topatzeko algoritmoak erabiliko dituen nodoen informazioa gordetzeko.
*/
typedef struct NODE {
	double f, g, h;
	int cost, block, neighboursCount, x, y, id;
	struct NODE* neighbours[8];
	struct NODE* parent;
} node;


int init(SDL_Window** win, SDL_Renderer** renderer);
//Menu
int mainMenu(SDL_Renderer* renderer, SDL_Surface** surface, int* clientState);
int checkButton(SDL_Rect btn);
int loadMenu(SDL_Surface** surface);

//Files
int exportMap(SDL_Surface* surface, SDL_Renderer* renderer);
void moveright(int start, char* path);
void showpath(char* path, SDL_Renderer* renderer, int position_path);
int formatuegokia(char* path);
int importMap(SDL_Surface** surface);


//Pathfinding
int loadMap(SDL_Surface* surface);
int checkColor(SDL_Color color, int x, int y);
SDL_Color getPixelColor(SDL_Surface* surface, int x, int y);
void updateMap(SDL_Color color);
void initNodes();
double calculateHValue(int ogX, int ogY, int destX, int destY);
int containsNode(node* element, int kop, node** list);
int aStar(SDL_Renderer* renderer);
void removeNode(node* element, int* kop, node** list);
void retracePath(node* start, node* end);
void printfPath(SDL_Renderer* renderer);
//Events
void eventHandler(SDL_Event e);

//Editor
void getTilePos(int* x, int* y);
int editor(SDL_Surface** surface, SDL_Renderer* renderer, int* clientState);
void renderGrid(SDL_Renderer* renderer);
int paintTile(SDL_Surface* surface, SDL_Color color);
int renderTilePreview(SDL_Renderer* renderer);
int loadEditorMenu(SDL_Surface** surface);
void renderEditor(SDL_Renderer* renderer, SDL_Surface* fileTexture);




#endif