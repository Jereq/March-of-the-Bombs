#include "Game.h"

int main(int argc, char** argv)
{
	Game::init(argc, argv);
	Game::getInstance()->start();

	return EXIT_SUCCESS;
}