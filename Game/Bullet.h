#pragma once

#include "Actor.h"

namespace nu {
	class Bullet : public Actor {
	public:
		Bullet() = default;

		Bullet( const Transform& transform, const Model& model, float speed = 700.0f, float lifespan = 2.0f );
	};
}