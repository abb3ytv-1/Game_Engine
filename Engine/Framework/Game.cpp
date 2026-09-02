#include "pch.h"
#include "Game.h"
#include "Scene.h"
#include "../Renderer/Renderer.h"

namespace nu {

	Game::Game() = default;
	Game::~Game() = default;

	void Game::SetScene(std::unique_ptr<Scene> scene) {
		a_scene = std::move(scene);
	}

	void Game::Update(float dt) {
		if (a_scene != nullptr) {
			a_scene->Update(dt);
		}
	}

	void Game::Draw(const Renderer& renderer) {
		if (a_scene != nullptr) {
			a_scene->Draw(renderer);
		}
	}
}