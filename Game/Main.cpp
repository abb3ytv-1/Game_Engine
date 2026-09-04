#include "../Engine/pch.h"

#include "../Engine/Engine.h"
#include "../Engine/Serialization/JsonSimple.h"
#include "../Engine/Core/File.h"
#include "../Engine/Core/Factory.h"
#include "../Engine/Framework/PlayerComponent.h"
#include "./Player.h"

#include "./FishGame.h"

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

    if (!SetWorkingDirectory("Assets"))
    {
        std::cerr << "Could not set Assets working directory.\n";
        return 1;
    }

    int result = 0;

    {
        RegisterPlayer();

        std::unique_ptr<FishGame> game = std::make_unique<FishGame>();
        result = game->Run();

        game.reset();
    }

    engine.Shutdown();

    return result;
}