#pragma once

#include "Component.h"

namespace nu {
	class BulletComponent : public Component {
	public:
		BulletComponent() = default;
		std::unique_ptr<Component> Clone() const override { return std::make_unique<BulletComponent>(); }
	};
}
