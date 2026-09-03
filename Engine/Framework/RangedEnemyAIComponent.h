#pragma once

#include "../Framework/Component.h"
#include "../Math/Vector2.h"

namespace nu {
	class RangedEnemyAIComponent : public Component {
	public:
		RangedEnemyAIComponent() = default;

		RangedEnemyAIComponent(
			const Actor* target,
			float speed = 80.0f,
			float preferredDistance = 350.0f,
			float shootInterval = 1.5f
		) :
			a_target(target),
			a_speed(speed),
			a_preferredDistance(preferredDistance),
			a_shootInterval(shootInterval)
		{}

		std::unique_ptr<Component> Clone() const override {
			auto clone = std::make_unique<RangedEnemyAIComponent>(
				a_target, a_speed, a_preferredDistance, a_shootInterval
			);
			return clone;
		}

		void Update(Actor& actor, float dt) override;

		// FishGame calls this each frame; returns true (once) when it's time to fire.
		bool ConsumeShootRequest() {
			if (a_wantsToShoot) {
				a_wantsToShoot = false;
				return true;
			}
			return false;
		}

		void SetTarget(const Actor* t) { a_target = t; }

	public:
		const Actor* a_target{ nullptr };
		float a_speed{ 80.0f };
		float a_preferredDistance{ 350.0f };
		float a_shootInterval{ 1.5f };

	private:
		float a_shootTimer{ 0.0f };
		bool a_wantsToShoot{ false };
	};
}