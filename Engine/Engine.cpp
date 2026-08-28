#include "pch.h"
#include "Engine.h"

namespace nu {
	// Defines the global engine declared with extern in Engine.h
	Engine engine;

	bool Engine::Initialize() {
		if (!a_renderer.Initialize("Game Engine", 1920, 1080)) {
			return false;
		}

		if (!a_input.Initialize()) {
			a_renderer.Shutdown();
			return false;
		}

		if (!a_audio.Initialize()) {
			a_input.Shutdown();
			a_renderer.Shutdown();
			return false;
		}

		if (!a_particleSystem.Initialize(5000)) {
			a_audio.Shutdown();
			a_input.Shutdown();
			a_renderer.Shutdown();

			return false;
		}

		a_physics.Initialize();

		a_time.Reset();

		return true;
	}

	void Engine::Shutdown() {
		Resources().RemoveAll();

		a_audio.Shutdown();
		a_input.Shutdown();
		a_renderer.Shutdown();
		a_physics.Shutdown();

	}

	void Engine::Update() {
		a_input.Update();
		a_time.Tick();
		a_audio.Update();

		a_particleSystem.Update(a_time.GetDeltaTime());
		a_physics.Update(a_time.GetDeltaTime());

	}
}