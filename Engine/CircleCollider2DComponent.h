#pragma once

#include "Component.h"

namespace nu {
	class CircleCollider2DComponent : public Component {
	public:
		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(float radius) : a_radius(radius) {}

		std::unique_ptr<Component> Clone() const override;

		void OnAttach(Actor& actor) override;

		float a_radius{ 1.0f };
	};
}
