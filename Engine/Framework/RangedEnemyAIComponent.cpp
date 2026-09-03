#include "pch.h"
#include "RangedEnemyAIComponent.h"
#include "../Framework/Actor.h"
#include "../Math/Vector2.h"

namespace nu {

	void RangedEnemyAIComponent::Update(Actor& actor, float dt) {
		if (a_target == nullptr) {
			return;
		}

		Vector2 toTarget =
			a_target->GetTransform().position - actor.GetTransform().position;

		float distance = toTarget.Length();

		Vector2 direction = (distance > 0.0f)
			? (toTarget / distance)
			: Vector2{ 0.0f, 0.0f };

		// Keep roughly a_preferredDistance away: close in if too far,
		// back off if too close, otherwise hold position.
		float distanceError = distance - a_preferredDistance;

		Vector2 velocity{ 0.0f, 0.0f };

		if (std::abs(distanceError) > 20.0f) {
			float moveSign = (distanceError > 0.0f) ? 1.0f : -1.0f;
			velocity = direction * (a_speed * moveSign);
		}

		actor.SetVelocity(velocity);

		a_shootTimer -= dt;

		if (a_shootTimer <= 0.0f) {
			a_shootTimer = a_shootInterval;
			a_wantsToShoot = true;
		}
	}

}