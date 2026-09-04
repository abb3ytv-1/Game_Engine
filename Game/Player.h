#pragma once

#include "../Engine/Framework/Actor.h"

namespace nu {
	class Player : public Actor {
	public:
		Player() = default;

		Player(
			const Transform& transform,
			const Model& model,
			float speed = 300.0f
		);

		void SetSpeed(float speed) {
			a_speed = speed;
		}

		float GetSpeed() const {
			return a_speed;
		}

		void SetAmmo(int ammo) {
			a_ammo = ammo;
		}

		int GetAmmo() const {
			return a_ammo;
		}

		void AddAmmo(int amount) {
			a_ammo += amount;
		}

	private:
		float a_speed{ 300.0f };
		int a_ammo{ 0 };
	};

	void RegisterPlayer();
}