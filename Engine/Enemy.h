#pragma once

#include "Actor.h"

namespace nu {
	class Enemy : public Actor {
	public:
		Enemy() = default;

		Enemy(
			const Transform& transform,
			const Model& model,
			float speed = 100.0f
		) :
			Actor{ transform, model },
			a_speed{ speed }
		{
			// This is multiplied by the enemy's scale.
			SetCollisionRadius(8.0f);
		}

		void Update(float dt) override;

		void SetTarget(const Actor& target) {
			a_target = &target;
		}

		void SetSpeed(float speed) {
			a_speed = speed;
		}

		float GetSpeed() const {
			return a_speed;
		}

	private:
		const Actor* a_target = nullptr;
		float a_speed = 100.0f;
	};
}