#include "pch.h"
#include "CircleCollider2DComponent.h"
#include "../Framework/Actor.h"

namespace nu
{
	std::unique_ptr<Component> CircleCollider2DComponent::Clone() const
	{
		return std::make_unique<CircleCollider2DComponent>(a_radius);
	}

	bool CircleCollider2DComponent::IsColliding(
		const Actor& actor,
		const CircleCollider2DComponent& other,
		const Actor& otherActor
	) const
	{
		Vector2 difference =
			otherActor.GetTransform().position -
			actor.GetTransform().position;

		float combinedRadius =
			(a_radius * actor.GetTransform().scale) +
			(other.a_radius * otherActor.GetTransform().scale);

		return difference.LengthSqr() <=
			combinedRadius * combinedRadius;
	}
}