#ifndef FILEMANAGER
#define FILEMANAGER
#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int exportMap(SDL_Surface* surface, SDL_Renderer* renderer);
void keydown(SDL_Event event, char* path, int* position_path, int* gorde, SDL_bool* done);
void moveright(int start, char* path);
SDL_Texture* showpath(char* path, SDL_Renderer* renderer, int position_path, SDL_Texture* Message);
int obtainPtrPosition(SDL_Renderer* renderer, TTF_Font* Verdana, int position_path, int i, char* path);
void renderpath(SDL_Renderer* renderer, SDL_Texture* Message, int ptrPos, int h, int w);
void exportMenu(SDL_Renderer* renderer);
SDL_Texture* paintbackground(SDL_Renderer* renderer);
int formatuegokia(char* path);
int importMap(SDL_Surface** surface, SDL_Renderer* renderer);
int importMap_menu(SDL_Surface** surface, SDL_Renderer* renderer);

#endif