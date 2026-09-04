#pragma once

#include "../Framework/Component.h"
#include "../Math/Vector2.h"

namespace nu {
    class RigidBodyComponent : public Component {
    public:
        RigidBodyComponent() = default;

        std::unique_ptr<Component> Clone() const override;

        void Update(Actor& actor, float dt) override;

        const Vector2& GetVelocity() const { return a_velocity; }
        void SetVelocity(const Vector2& v) { a_velocity = v; }

        float GetDamping() const { return a_damping; }
        void SetDamping(float d) { a_damping = d; }

        float GetGravity() const { return a_gravity; }
        void SetGravity(float gravity) { a_gravity = gravity; }

        bool IsGrounded() const { return a_grounded; }
        void SetGrounded(bool grounded) { a_grounded = grounded; }

        void Jump(float force);

    private:
        Vector2 a_velocity{ 0.0f, 0.0f };
        float a_damping{ 1.0f };

        float a_gravity{ 980.0f };
        bool a_grounded{ false };
    };
}