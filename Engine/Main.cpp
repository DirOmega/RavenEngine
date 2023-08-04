#include "Game.h"

Game *pGame = 0;

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	pGame = new Game("Raven Engine",800,600);
	pGame->Run();

	delete pGame;

	return 0;
};