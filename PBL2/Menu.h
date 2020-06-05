#ifndef MENU
#define MENU
#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int init(SDL_Window** win, SDL_Renderer** renderer);
//Menu
int mainMenu(SDL_Renderer* renderer, SDL_Surface** surface, int* clientState);
int checkButton(SDL_Rect btn);
int loadMenu(SDL_Surface** surface);
void drawIMG(SDL_Renderer* renderer, char* path, SDL_Rect btn);

#endif