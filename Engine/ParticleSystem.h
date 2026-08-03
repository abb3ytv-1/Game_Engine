#pragma once

#include "Vector2.h"
#include "Vector3.h"

#include <cstddef>
#include <vector>

namespace nu {
	struct Particle {
		bool active{ false };
		float lifespan{ 1.0f };

		Vector2 position{ 0.0f, 0.0f };
		Vector2 velocity{ 0.0f, 0.0f };

		Color color{ 0.0f, 0.0f, 0.0f };
	};

	class Renderer;

	class ParticleSystem {
	public:
		ParticleSystem() = default;

		bool Initialize(
			std::size_t poolSize = 1000
		);

		void Shutdown();

		void Update(float dt);

		void Draw(
			const Renderer& renderer
		);

		void AddParticle(
			const Particle& particle
		);

	private:
		Particle* GetFreeParticle();

	private:
		std::vector<Particle> a_particles;
	};
}