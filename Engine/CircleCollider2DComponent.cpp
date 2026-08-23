#include "pch.h"
#include "CircleCollider2DComponent.h"
#include "Actor.h"

namespace nu {

	std::unique_ptr<Component> CircleCollider2DComponent::Clone() const {
		return std::make_unique<CircleCollider2DComponent>(a_radius);
	}

	void CircleCollider2DComponent::OnAttach(Actor& actor) {
		actor.SetCollisionRadius(a_radius);
	}
}
