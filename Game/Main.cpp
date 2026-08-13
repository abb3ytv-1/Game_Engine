#include "pch.h"
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

    rapidjson::Document document;
    if (json::Load("json.txt", document))
    {
        std::string name;
        int age = 0;
        float speed = 0.0f;
        bool active = false;
        nu::Vector2 position;
        nu::Vector3 color;

        json::Read(document, "name", name);
        json::Read(document, "age", age);
        json::Read(document, "speed", speed);
        json::Read(document, "active", active);
        json::Read(document, "position", position);
        json::Read(document, "color", color);

        std::cout << "name: " << name << std::endl;
        std::cout << "age: " << age << std::endl;
        std::cout << "speed: " << speed << std::endl;
        std::cout << "active: " << (active ? "true" : "false") << std::endl;
        std::cout << "position: (" << position.x << ", " << position.y << ")" << std::endl;
        std::cout << "color: (" << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
    }

	int result = 0;

	{
		SpaceGame game;
		result = game.Run();
	}

	engine.Shutdown();

	return result;
}