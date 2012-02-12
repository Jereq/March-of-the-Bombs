#include "Game.h"

int main(int argc, char** argv)
{
	Game::ptr game = Game::getInstance();
	game->start();

	return EXIT_SUCCESS;
}