#include "pch.h"
#include "RigidBodyComponent.h"
#include "../Framework/Actor.h"

namespace nu {

    std::unique_ptr<Component> RigidBodyComponent::Clone() const {
        return std::make_unique<RigidBodyComponent>(*this);
    }

    void RigidBodyComponent::Update(Actor& actor, float dt) {

        // Apply gravity when the actor is not standing on something.
        if (!a_grounded) {
            a_velocity.y += a_gravity * dt;
        }

        // Move the actor.
        actor.SetTransform(
            Transform{
                actor.GetTransform().position + a_velocity * dt,
                actor.GetTransform().rotation,
                actor.GetTransform().scale
            }
        );

        // Apply horizontal damping.
        a_velocity.x *= (1.0f / (1.0f + (a_damping * dt)));
    }

    void RigidBodyComponent::Jump(float force) {
        if (a_grounded) {
            a_velocity.y = -force;
            a_grounded = false;
        }
    }
}