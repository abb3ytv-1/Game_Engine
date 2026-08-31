#include "../Engine/pch.h"

#include "../Engine/Engine.h"
#include "../Engine/Serialization/JsonSimple.h"
#include "../Engine/Core/File.h"
#include "../Engine/Core/Factory.h"
#include "../Engine/Framework/PlayerComponent.h"

#include "Space_Game.h"

#include <filesystem>
#include <rapidjson/document.h>
#include <fstream>
#include <iostream>
#include <memory>

using namespace nu;

int main()
{
    if (!engine.Initialize())
    {
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