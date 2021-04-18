#include <iostream>

#include "Game/Game.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, ".UTF8");

	Game game;
	int result = game.Run();
	return result;
}