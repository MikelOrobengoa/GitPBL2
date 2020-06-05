#include "General.h"
#include "PathFinding.h"
#include <math.h>
#include <time.h>

int robotSim(SDL_Renderer* renderer) {

	static clock_t endTime = 0, startTime = 0;
	//printf("end %f, start %f\n", endTime, startTime);

	if (startTime == 0) startTime = clock();
	if (endTime == 0) {
		endTime = clock();
	}
	else {
		endTime = clock();
	}
	double delta = ((double)endTime - startTime) / CLOCKS_PER_SEC;
	startTime = endTime;

	static SDL_Texture* botTexture;
	if (!botTexture) {
		SDL_Surface* surf = NULL;
		if (!loadIMG(&surf, "images/robotin.png")) return 0;
		else {
			botTexture = SDL_CreateTextureFromSurface(renderer, surf);
			SDL_FreeSurface(surf);
		}
	}

	static int pathKop, stage;
	static node** path;

	static int currentNode;
	static float x, y;
	int xDir = 1, yDir = 1;

	switch (stage) {
	case 0:
		path = getCurrentPath(&pathKop);
		currentNode = pathKop - 1;
		x = path[currentNode]->x * TILESIZE;
		y = path[currentNode]->y * TILESIZE + TILESIZE;
		currentNode--;
		stage = 1;
		break;
	case 1:
		if(moveBot(&x, &y, &xDir, &yDir, *path[currentNode], delta))
			currentNode--;
		if (currentNode < 0) {
			stage = 2;
			currentNode++;
		}
		break;
	case 2:
		if(moveBot(&x, &y, &xDir, &yDir, *path[currentNode], delta))
			currentNode++;
		if (currentNode == pathKop) {
			stage = 0;
		}
		break;
	}
	
	drawBot(renderer, botTexture, x, y, xDir, yDir);

	if (stage == 0) {
		SDL_DestroyTexture(botTexture);
		botTexture = NULL;
	}
	return stage;
}

int moveBot(float* x, float* y, int* xDir, int* yDir, node next, double delta) {
	int speed = 100, reached = 0;
	if (*x < next.x * TILESIZE) {
		*xDir = 1;
		*x += speed * delta;
		if (*x > next.x * TILESIZE) *x = next.x * TILESIZE;
	}
	else if (*x > next.x * TILESIZE) {
		*xDir = -1;
		*x -= speed * delta;
		if (*x < next.x * TILESIZE) *x = next.x * TILESIZE;
	}

	if (*y < next.y * TILESIZE + TILESIZE) {
		*yDir = 1;
		*y += speed * delta;
		if (*y > next.y * TILESIZE + TILESIZE) *y = next.y * TILESIZE + TILESIZE;
	}
	else if (*y > next.y * TILESIZE + TILESIZE) {
		*yDir = -1;
		*y -= speed * delta;
		if (*y < next.y * TILESIZE + TILESIZE) *y = next.y * TILESIZE + TILESIZE;
	}

	if (*x == next.x * TILESIZE && *y == next.y * TILESIZE + TILESIZE) reached = 1;
	return reached;
}

void drawBot(SDL_Renderer* renderer, SDL_Texture* tex, int x, int y, int xDir, int yDir) {
	SDL_Rect r = { x, y, TILESIZE, TILESIZE };
	double angle = 0;
	if (xDir == 1 && yDir == 1) angle = 0;
	else if (xDir == 1 && yDir == 0) angle = -45;
	else if (xDir == 0 && yDir == 1) angle = 45;
	else if (xDir == 0 && yDir == -1) angle = -135;
	else if (xDir == -1 && yDir == 0) angle = 45;
	else if (xDir == -1 && yDir == -1) angle = 90;
	else if (xDir == 1 && yDir == -1) angle = -90;
	else if (xDir == -1 && yDir == 1) angle = 0;
	
	int flip = SDL_FLIP_NONE;
	if (xDir == -1) flip = SDL_FLIP_HORIZONTAL;
	SDL_RenderCopyEx(renderer,
		tex,
		NULL,
		&r,
		angle,
		NULL,
		flip);
}