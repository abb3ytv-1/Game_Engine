#include "pch.h"
#include "Enemy.h"

namespace nu {
	void Enemy::Update(float dt) {
		if (a_target != nullptr) {
			Vector2 direction =
				a_target->GetTransform().position -
				a_transform.position;

			if (direction.LengthSqr() > 0.0f) {
				direction = direction.Normalized();
			}

			a_velocity = direction * a_speed;
		}

		Actor::Update(dt);
	}
}