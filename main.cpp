#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>
#include <cmath>
#include <vector>
#include <list>

#include "headers/globals.hpp"
#include "headers/pathfinder.hpp"

// A* pathfinder
// commands:
// select the action you want to do with the left mouse click.
// left click on the screen to place the objects (you, target, walls); you can either click or hold the mouse to draw.
// right click on the screen to erase the objects(you, target, walls); you can either click or hold the mouse to erase.
// There cannot be more than one start or end point.
// <<enter>> to start finding.
// <<esc>> to quit.

int main(int argc, char* args[]){

	InitializeSDL();

	Pathfinder A_star;

	A_star.Cycle();

	CloseSDL();

	return 0;

}
