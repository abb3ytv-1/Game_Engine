#pragma once

#include "Component.h"
#include "Vector2.h"

namespace nu {
	class RigidBodyComponent : public Component {
	public:
		RigidBodyComponent() = default;

		std::unique_ptr<Component> Clone() const override;

		void Update(Actor& actor, float dt) override;

		// Velocity and damping accessors
		const Vector2& GetVelocity() const { return a_velocity; }
		void SetVelocity(const Vector2& v) { a_velocity = v; }

		float GetDamping() const { return a_damping; }
		void SetDamping(float d) { a_damping = d; }

	private:
		Vector2 a_velocity{ 0.0f, 0.0f };
		float a_damping{ 1.0f };
	};
}
