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
Biderik motzena topatzeko algoritmoak erabiliko dituen nodoen informazioa gordetzeko.
*/
typedef struct NODE {
	double f, g, h;
	int cost, block, neighboursCount, x, y, id;
	struct NODE* neighbours[8];
	struct NODE* parent;
} node;

//Pathfinding
int loadMap(SDL_Surface* surface);
int checkColor(SDL_Surface* surface, SDL_Color color, int x, int y);
SDL_Color getPixelColor(SDL_Surface* surface, int x, int y);
void updateMap(SDL_Surface* surface, SDL_Color color);
void initNodes(int diags);
double calculateHValue(int ogX, int ogY, int destX, int destY, int diags);
int containsNode(node* element, int kop, node** list);
int findPath(SDL_Renderer* renderer, int alg, int diags);
void removeNode(node* element, int* kop, node** list);
void retracePath(node* start, node* end);
void printfPath(SDL_Renderer* renderer);
int memoriaKudeatu(node** ptr, int kop);

//Simulation
int robotSim(SDL_Renderer* renderer);
int moveBot(float* x, float* y, int* xDir, int* yDir, node next, double delta);
void drawBot(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y, int xDir, int yDir);
node** getCurrentPath(int* kop);

#endif