#include "pch.h"
#include "Actor.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Component.h"

namespace nu {
	void Actor::Update(float dt) {
		if (a_destroyed) {
			return;
		}

		// Update lifespan when one has been set.
		// A negative lifespan means the actor lives forever.
		if (a_lifespan > 0.0f) {
			a_lifespan -= dt;

			if (a_lifespan <= 0.0f) {
				Destroy();
				return;
			}
		}

		// Physics is handled by RigidBodyComponent when attached to the actor.

		// Wrap actors around the screen
		a_transform.position.x = Wrap(
			0.0f,
			1920.0f,
			a_transform.position.x
		);

		a_transform.position.y = Wrap(
			0.0f,
			1080.0f,
			a_transform.position.y
		);

		// Component updates
		for (auto& comp : a_components) {
			if (comp) comp->Update(*this, dt);
		}
	}

	void Actor::Draw(
		const Renderer& renderer
	) const {
		if (a_destroyed) {
			return;
		}

		// Draw model only if it contains meshes. Models may be empty when visuals
		// are provided by SpriteRendererComponent or when model geometry is disabled.
		if (!a_model.GetMeshes().empty()) {
			renderer.DrawModel(
				a_model,
				a_transform
			);
		}

		// Component drawing
		for (auto& comp : a_components) {
			if (comp) comp->Draw(renderer, *this);
		}
	}


	// Add a component and notify it
	void Actor::AddComponent(std::unique_ptr<Component> comp) {
		if (!comp) return;
		comp->OnAttach(*this);
		a_components.push_back(std::move(comp));
	}

	Actor::Actor(const Actor& other) {
		// copy plain data
		a_transform = other.a_transform;
		a_velocity = other.a_velocity;
		a_damping = other.a_damping;
		a_lifespan = other.a_lifespan;
		// textures are handled by SpriteRendererComponent now
		a_collisionRadius = other.a_collisionRadius;
		a_destroyed = other.a_destroyed;
		a_model = other.a_model;

		// deep copy components
		for (auto& comp : other.a_components) {
			if (comp) {
				a_components.push_back(comp->Clone());
			}
		}
	}

	Actor& Actor::operator=(const Actor& other) {
		if (this == &other) return *this;

		// copy plain data
		a_transform = other.a_transform;
		a_velocity = other.a_velocity;
		a_damping = other.a_damping;
		a_lifespan = other.a_lifespan;
		// textures are handled by SpriteRendererComponent now
		a_collisionRadius = other.a_collisionRadius;
		a_destroyed = other.a_destroyed;
		a_model = other.a_model;

		// deep copy components
		a_components.clear();
		for (auto& comp : other.a_components) {
			if (comp) {
				a_components.push_back(comp->Clone());
			}
		}

		return *this;
	}

	bool Actor::IsColliding(
		const Actor& other
	) const {
		Vector2 difference =
			other.GetTransform().position -
			a_transform.position;

		float combinedRadius =
			GetCollisionRadius() +
			other.GetCollisionRadius();

		return (
			difference.LengthSqr() <=
			combinedRadius * combinedRadius
			);
	}
}