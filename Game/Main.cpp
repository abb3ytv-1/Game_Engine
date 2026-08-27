#include "pch.h"
#include "../Engine/Engine.h"
#include "../Engine/JsonSimple.h"
#include "Space_Game.h"

#include <filesystem>
#include "../Engine/File.h"
#include "../Engine/Factory.h"
#include "../Engine/PlayerComponent.h"
#include <rapidjson/document.h>
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
		using namespace nu;

	{
		SpaceGame game;
		result = game.Run();
	}

	engine.Shutdown();

	return result;
}