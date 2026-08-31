#include "pch.h"
#include "ParticleSystem.h"

#include "Renderer.h"
#include "../Math/Transform.h"
#include "Texture.h"

#include <algorithm>
#include <cstdint>

using namespace nu;

namespace {
	std::uint8_t ToColorChannel(float value) {
		float clampedValue =
			std::clamp(value, 0.0f, 1.0f);

		return static_cast<std::uint8_t>(
			clampedValue * 255.0f
			);
	}
}

bool ParticleSystem::Initialize(
	std::size_t poolSize
) {
	a_particles.clear();
	a_particles.resize(poolSize);

	return true;
}

void ParticleSystem::Shutdown() {
	a_particles.clear();
}

void ParticleSystem::Update(float dt) {
	for (Particle& particle : a_particles) {
		if (!particle.active) {
			continue;
		}

		particle.lifespan -= dt;

		particle.active =
			particle.lifespan > 0.0f;

		if (!particle.active) {
			continue;
		}

		particle.position +=
			particle.velocity * dt;

		// update rotation
		particle.rotation += particle.angularVelocity * dt;
	}
}

void ParticleSystem::Draw(
	const Renderer& renderer
) {
	for (const Particle& particle : a_particles) {
		if (!particle.active) {
			continue;
		}

		// Compute life ratio for fading/scaling
		float lifeRatio = 1.0f;
		if (particle.initialLifespan > 0.0f) {
			lifeRatio = std::clamp(
				particle.lifespan / particle.initialLifespan,
				0.0f,
				1.0f
			);
		}

		Uint8 alpha = static_cast<Uint8>(lifeRatio * 255.0f);

		// If we have a particle texture, render textured quads with alpha and scaling
		if (a_texture != nullptr && a_texture->GetSize().x > 0.0f) {
			Vector2 texSize = a_texture->GetSize();
			float scale = (particle.size / texSize.x) * lifeRatio; // shrink over life

			Transform t{
				particle.position,
				particle.rotation,
				1.0f
			};

			renderer.DrawTexture(*a_texture, t, Vector2{ scale, scale }, Vector2{ 0.5f, 0.5f }, lifeRatio);
		}
		else {
			renderer.SetColor(
				ToColorChannel(particle.color.r),
				ToColorChannel(particle.color.g),
				ToColorChannel(particle.color.b),
				alpha
			);

			renderer.DrawPoint(
				particle.position.x,
				particle.position.y
			);
		}
	}
}

void ParticleSystem::AddParticle(
	const Particle& particle
) {
	Particle* freeParticle =
		GetFreeParticle();

	if (freeParticle == nullptr) {
		return;
	}

	*freeParticle = particle;
	freeParticle->active = true;

	// Record initial lifespan for fading/scaling
	if (freeParticle->lifespan > 0.0f) {
		freeParticle->initialLifespan = freeParticle->lifespan;
	}

	// Ensure particle size has a reasonable default
	if (freeParticle->size <= 0.0f) {
		freeParticle->size = 8.0f;
	}
}

Particle* ParticleSystem::GetFreeParticle() {
	for (Particle& particle : a_particles) {
		if (!particle.active) {
			return &particle;
		}
	}

	return nullptr;
}