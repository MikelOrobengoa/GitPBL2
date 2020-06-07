#ifndef GENERAL
#define GENERAL
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
extern int sim_diags, sim_alg;

enum button {new, import, export, Red, Green, Blue, Minus, Cost, Plus, alg, diag, Play, Help};

//Events
void eventHandler(SDL_Event e);


#endif