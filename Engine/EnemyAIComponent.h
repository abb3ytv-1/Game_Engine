#pragma once

#include "Component.h"

namespace nu {
	class EnemyAIComponent : public Component {
	public:
		EnemyAIComponent() = default;
		EnemyAIComponent(const Actor* target, float speed = 100.0f) : a_target(target), a_speed(speed) {}

		std::unique_ptr<Component> Clone() const override {
			return std::make_unique<EnemyAIComponent>(a_target, a_speed);
		}

		void Update(Actor& actor, float dt) override;

		void SetTarget(const Actor* t) { a_target = t; }
		void SetSpeed(float s) { a_speed = s; }

		const Actor* a_target{ nullptr };
		float a_speed{ 100.0f };
	};
}
