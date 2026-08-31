#pragma once

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

#include <cstddef>
#include <vector>
#include <memory>

namespace nu {
	class Texture;
}

namespace nu {
	struct Particle {
		bool active{ false };
		float lifespan{ 1.0f };

		// initial lifespan to compute fading/scale
		float initialLifespan{ 1.0f };

		// particle render size in pixels
		float size{ 8.0f };

		Vector2 position{ 0.0f, 0.0f };
		Vector2 velocity{ 0.0f, 0.0f };

		Color color{ 0.0f, 0.0f, 0.0f };
		// rotation (degrees) and angular velocity (degrees/sec)
		float rotation{ 0.0f };
		float angularVelocity{ 0.0f };
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

		// Optional sprite texture for particles
		void SetTexture(const std::shared_ptr<Texture>& texture) { a_texture = texture; }

		void AddParticle(
			const Particle& particle
		);

	private:
		Particle* GetFreeParticle();

	private:
		std::vector<Particle> a_particles;
		std::shared_ptr<Texture> a_texture;
	};
}