#include "pch.h"
#include "ParticleSystem.h"

#include "Renderer.h"

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
	}
}

void ParticleSystem::Draw(
	const Renderer& renderer
) {
	for (const Particle& particle : a_particles) {
		if (!particle.active) {
			continue;
		}

		renderer.SetColor(
			ToColorChannel(particle.color.r),
			ToColorChannel(particle.color.g),
			ToColorChannel(particle.color.b),
			255
		);

		renderer.DrawPoint(
			particle.position.x,
			particle.position.y
		);
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
}

Particle* ParticleSystem::GetFreeParticle() {
	for (Particle& particle : a_particles) {
		if (!particle.active) {
			return &particle;
		}
	}

	return nullptr;
}