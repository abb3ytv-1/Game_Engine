#include "../Engine/Engine.h"
#include "Space_Game.h"

#include <fstream>
#include <iostream>
#include <memory>

using namespace nu;

int main() {
	if (!engine.Initialize()) {
		return 1;
	}

	int result = 0;

	{
		SpaceGame game;
		result = game.Run();
	}

	engine.Shutdown();

	return result;
}