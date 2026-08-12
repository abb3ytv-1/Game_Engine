#include "pch.h"
#include "Actor.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Texture.h"

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

		// Physics
		a_transform.position += a_velocity * dt;

		a_velocity *= (
			1.0f /
			(1.0f + (a_damping * dt))
			);

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
	}

	void Actor::Draw(
		const Renderer& renderer
	) const {
		if (a_destroyed) {
			return;
		}

		if (a_texture != nullptr) {
			renderer.DrawTexture(
				*a_texture,
				a_transform,
				a_textureScale,
				a_textureOrigin
			);

			return;
		}

		renderer.DrawModel(
			a_model,
			a_transform
		);
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