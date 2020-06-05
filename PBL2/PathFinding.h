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
void drawIMG(SDL_Renderer* renderer, char* path, SDL_Rect btn);

//Files
int exportMap(SDL_Surface* surface, SDL_Renderer* renderer);
void keydown(SDL_Event event, char* path, int* position_path, int* gorde, SDL_bool* done);
void moveright(int start, char* path);
SDL_Texture* showpath(char* path, SDL_Renderer* renderer, int position_path, SDL_Texture* Message);
int obtainPtrPosition(SDL_Renderer* renderer, TTF_Font* Verdana, int position_path, int i, char* path);
void renderpath(SDL_Renderer* renderer, SDL_Texture* Message, int ptrPos, int h, int w);
void exportMenu(SDL_Renderer* renderer);
SDL_Texture* paintbackground(SDL_Renderer* renderer);
void loadTexture(SDL_Texture** Tex, SDL_Renderer* renderer, char* path);
int formatuegokia(char* path);
int importMap(SDL_Surface** surface, SDL_Renderer* renderer);
int importMap_menu(SDL_Surface** surface, SDL_Renderer* renderer);


//Pathfinding
int loadMap(SDL_Surface* surface);
int checkColor(SDL_Surface* surface, SDL_Color color, int x, int y);
SDL_Color getPixelColor(SDL_Surface* surface, int x, int y);
void updateMap(SDL_Surface* surface, SDL_Color color);
void initNodes();
double calculateHValue(int ogX, int ogY, int destX, int destY);
int containsNode(node* element, int kop, node** list);
int aStar(SDL_Renderer* renderer);
void removeNode(node* element, int* kop, node** list);
void retracePath(node* start, node* end);
void printfPath(SDL_Renderer* renderer);

//BotSim
int robotSim(SDL_Renderer* renderer);
int moveBot(float* x, float* y, int* xDir, int* yDir, node next, double delta);
void drawBot(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y, int xDir, int yDir);
node** getCurrentPath(int* kop);



//Events
void eventHandler(SDL_Event e);

//Editor
void getTilePos(int* x, int* y);
int editor(SDL_Surface** surface, SDL_Renderer* renderer, int* clientState);
void renderGrid(SDL_Renderer* renderer);
int paintTile(SDL_Surface* surface, SDL_Color color);
int renderTilePreview(SDL_Renderer* renderer);
int loadEditorMenu(SDL_Surface** surface);
void renderEditor(SDL_Renderer* renderer, SDL_Texture* fileTexture);
int loadIMG(SDL_Surface** surface, char* image_path);
void loadCost(SDL_Renderer* renderer, SDL_Texture** c0, SDL_Texture** c0_click, SDL_Texture** c1, SDL_Texture** c1_click, SDL_Texture** c2, SDL_Texture** c2_click, SDL_Texture** c3, SDL_Texture** c3_click, SDL_Texture** c4, SDL_Texture** c4_click);
int helpmenu(SDL_Renderer* renderer);



#endif