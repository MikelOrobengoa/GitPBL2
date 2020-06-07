#ifndef EDITOR
#define EDITOR
#define _CRT_SECURE_NO_WARNINGS
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

void getTilePos(int* x, int* y);
int editor(SDL_Surface** surface, SDL_Renderer* renderer, int* clientState);
void renderGrid(SDL_Renderer* renderer);
int paintTile(SDL_Surface* surface, SDL_Color color);
int renderTilePreview(SDL_Renderer* renderer);
int loadEditorMenu(SDL_Surface** surface);
void loadTexture(SDL_Texture** Tex, SDL_Renderer* renderer, char* path);
void renderEditor(SDL_Renderer* renderer, SDL_Texture* fileTexture);
int loadIMG(SDL_Surface** surface, char* image_path);
void loadCost(SDL_Renderer* renderer, SDL_Texture** c0, SDL_Texture** c0_click, SDL_Texture** c1, SDL_Texture** c1_click, SDL_Texture** c2, SDL_Texture** c2_click, SDL_Texture** c3, SDL_Texture** c3_click, SDL_Texture** c4, SDL_Texture** c4_click);
int helpmenu(SDL_Renderer* renderer);

#endif