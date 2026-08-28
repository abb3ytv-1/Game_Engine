#pragma once

#include "Audio.h"
#include "GameTime.h"
#include "Input.h"
#include "ParticleSystem.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "StringUtils.h"
#include "Physics.h"


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