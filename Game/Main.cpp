#include "pch.h"
#include "../Engine/Engine.h"
#include "../Engine/JsonSimple.h"
#include "Space_Game.h"

#include <filesystem>
#include "../Engine/File.h"
#include "../Engine/Factory.h"
#include "Player.h"
#include "../Engine/PlayerComponent.h"

#include <fstream>
#include <iostream>
#include <memory>

using namespace nu;

int main() {
	if (!engine.Initialize()) {
		return 1;
	}



	int result = 0;

	// Demo: register Player, create via Factory, and print basic info
	{
		using namespace nu;

		// Demo: create a composed Actor with PlayerComponent and print basic info
		using namespace nu;
		std::unique_ptr<Actor> actor = std::make_unique<Actor>();
		actor->AddComponent(std::make_unique<nu::PlayerComponent>(300.0f, 0));

		bool isActor = (actor != nullptr);
		auto* pc = actor->GetComponent<nu::PlayerComponent>();
		float radius = isActor ? actor->GetCollisionRadius() : 0.0f;

		std::cout << "Demo: IsActor: " << (isActor ? "true" : "false") << "\n";
		std::cout << "Demo: IsPlayerComponent: " << (pc ? "true" : "false") << "\n";
		std::cout << "Demo: GetRadius(): " << radius << "\n";
	}

	{
	json::Document doc;
	std::string found;
	{
		std::string wd = nu::GetWorkingDirectory();
		std::filesystem::path p(wd);
		auto tryPath = [&](const std::filesystem::path &pp) -> bool {
			std::error_code ec;
			if (std::filesystem::exists(pp, ec) && !ec) { found = pp.string(); return true; }
			return false;
		};

		if (!tryPath(p / std::filesystem::path("json.txt"))) {
			if (!tryPath(p / std::filesystem::path("Assets") / std::filesystem::path("json.txt"))) {
				while (true) {
					if (tryPath(p / std::filesystem::path("Assets") / std::filesystem::path("json.txt"))) break;
					if (tryPath(p / std::filesystem::path("json.txt"))) break;
					if (p.has_parent_path()) p = p.parent_path(); else break;
				}
			}
		}
	}

	if (found.empty()) {
		std::cout << "Could not find json.txt in working directory or Assets folders.\n";
	}
	else if (json::Load(found, doc)) {
		std::cout << "Loaded JSON from: " << found << "\n";
			std::string name;
			int age = 0;
			float speed = 0.0f;
			bool active = false;
			nu::Vector2 position;
			nu::Vector3 color;

			json::Read(doc, "name", name);
			json::Read(doc, "age", age);
			json::Read(doc, "speed", speed);
			json::Read(doc, "active", active);
			json::Read(doc, "position", position);
			json::Read(doc, "color", color);

			std::cout << "name: " << name << std::endl;
			std::cout << "age: " << age << std::endl;
			std::cout << "speed: " << speed << std::endl;
			std::cout << "active: " << (active ? "true" : "false") << std::endl;
			std::cout << "position: (" << position.x << ", " << position.y << ")" << std::endl;
			std::cout << "color: (" << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
		}
		else {
			std::cout << "Could not load json.txt with JsonSimple" << std::endl;
		}
	}

	{
		SpaceGame game;
		result = game.Run();
	}

	engine.Shutdown();

	return result;
}