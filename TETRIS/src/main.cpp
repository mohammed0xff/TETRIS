#include "game.h"


int main(int argc, char* argv[])
{
	Game game;
	if(game.init())
		game.stateLoop();

    return 0;
}