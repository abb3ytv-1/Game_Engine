#pragma once

#include "Audio/Audio.h"
#include "Core/GameTime.h"
#include "Input/Input.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Core/StringUtils.h"
#include "Physics/Physics.h"


namespace nu {
	class Engine {
	public:
		Engine() = default;

		bool Initialize();
		void Shutdown();
		void Update();

		Input& GetInput() {
			return a_input;
		}

		Renderer& GetRenderer() {
			return a_renderer;
		}

		Time& GetTime() {
			return a_time;
		}

		Audio& GetAudio() {
			return a_audio;
		}

		ParticleSystem& GetPS() {
			return a_particleSystem;
		}

		Physics& GetPhysics() { return a_physics; }


	private:
		Input a_input;
		Renderer a_renderer;
		Time a_time;
		Audio a_audio;
		ParticleSystem a_particleSystem;
		Physics a_physics;

	};

	extern Engine engine;
}