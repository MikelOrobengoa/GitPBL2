#include "PathFinding.h"

/*
Teklak sakatuta dauden ala ez. Enum -> SDL_Keycode
*/
int KEYS[322];

/*
Saguaren klika sakatuta dagoen ala ez.
*/
int MOUSE_CLICK = 0;

/*
Ebentoak administratzeko. Aktibatu edo desaktibatu diren teklak gorde. Gauza bera saguaren klikarekin.
*/
void eventHandler(SDL_Event e) {
    switch (e.type) {
    case SDL_KEYDOWN:
        *(KEYS + e.key.keysym.sym) = 1;
        break;
    case SDL_KEYUP:
        *(KEYS + e.key.keysym.sym) = 0;
        break;
    case SDL_MOUSEBUTTONDOWN:
        MOUSE_CLICK = 1;
        break;
    case SDL_MOUSEBUTTONUP:
        MOUSE_CLICK = 0;
        break;
    default: break;
    }
}