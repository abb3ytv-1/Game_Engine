#include "pch.h"
#include "Enemy.h"
#include "Factory.h"
#include "RigidBodyComponent.h"

namespace nu {
	void Enemy::Update(float dt) {
		if (a_target != nullptr) {
			Vector2 direction =
				a_target->GetTransform().position -
				a_transform.position;

			if (direction.LengthSqr() > 0.0f) {
				direction = direction.Normalized();
			}

			SetVelocity(direction * a_speed);
		}

		Actor::Update(dt);
	}

	namespace {
		const bool g_enemyRegistered =
			nu::Factory::Instance().Register(
				"Enemy",
				std::make_unique<nu::Creator<nu::Enemy>>()
			);
	}
}