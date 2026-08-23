#include "pch.h"
#include "RigidBodyComponent.h"
#include "Actor.h"

namespace nu {

	std::unique_ptr<Component> RigidBodyComponent::Clone() const {
		// Use copy-construction to ensure all internal members (including internal velocity) are copied.
		auto c = std::make_unique<RigidBodyComponent>(*this);
		return c;
	}

	void RigidBodyComponent::Update(Actor& actor, float dt) {
		// Move actor using this component's velocity and apply damping.
		actor.SetTransform(Transform{ actor.GetTransform().position + a_velocity * dt, actor.GetTransform().rotation, actor.GetTransform().scale });

		a_velocity *= (1.0f / (1.0f + (a_damping * dt)));

		// Ensure actor-level getters/setters reflect this component's velocity if requested
		// (Actor forwards to RigidBodyComponent when present)
	}
}
