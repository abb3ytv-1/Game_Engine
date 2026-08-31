#pragma once

#include "Component.h"

namespace nu {
	class PlayerComponent : public Component {
	public:
		PlayerComponent() = default;
		PlayerComponent(float speed, int ammo = 0) : a_speed(speed), a_ammo(ammo) {}

		std::unique_ptr<Component> Clone() const override {
			return std::make_unique<PlayerComponent>(a_speed, a_ammo);
		}

		float GetSpeed() const { return a_speed; }
		void SetSpeed(float s) { a_speed = s; }

		int GetAmmo() const { return a_ammo; }
		void SetAmmo(int v) { a_ammo = v; }
		void AddAmmo(int v) { a_ammo += v; }

		float a_speed{ 300.0f };
		int a_ammo{ 0 };
	};
}
