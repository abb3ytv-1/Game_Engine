#include "pch.h"
#include "EnemyAIComponent.h"
#include "Actor.h"

namespace nu {

	void EnemyAIComponent::Update(Actor& actor, float dt) {
		if (a_target == nullptr) return;

		Vector2 direction = a_target->GetTransform().position - actor.GetTransform().position;
		if (direction.LengthSqr() > 0.0f) {
			direction = direction.Normalized();
		}

		actor.SetVelocity(direction * a_speed);
	}

}
