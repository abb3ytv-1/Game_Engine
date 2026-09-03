#pragma once

#include "Component.h"

namespace nu {
	class BulletComponent : public Component {
	public:
		BulletComponent() = default;
		explicit BulletComponent(bool isEnemyOwned) : a_isEnemyOwned(isEnemyOwned) {}

		std::unique_ptr<Component> Clone() const override {
			return std::make_unique<BulletComponent>(a_isEnemyOwned);
		}

		bool IsEnemyOwned() const { return a_isEnemyOwned; }

	private:
		bool a_isEnemyOwned{ false };
	};
}