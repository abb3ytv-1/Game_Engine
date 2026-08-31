#pragma once

#include "../Framework/Component.h"


namespace nu
{
	class CircleCollider2DComponent : public Component
	{
	public:
		CircleCollider2DComponent() = default;

		explicit CircleCollider2DComponent(float radius)
			: a_radius(radius)
		{}

		std::unique_ptr<Component> Clone() const override;

		float GetRadius() const
		{
			return a_radius;
		}

		void SetRadius(float radius)
		{
			a_radius = radius;
		}

		bool IsColliding(
			const Actor& actor,
			const CircleCollider2DComponent& other,
			const Actor& otherActor
		) const;

	private:
		float a_radius{ 1.0f };
	};
}