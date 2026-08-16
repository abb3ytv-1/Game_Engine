#include "Bullet.h"

#include "MathUtils.h"
#include "../Engine/Factory.h"

using namespace nu;

Bullet::Bullet(
	const Transform& transform,
	const Model& model,
	float speed,
	float lifespan
) :
	Actor{ transform, model }
{
	// Models face toward positive x.
	Vector2 forward{ 1.0f, 0.0f };

	forward = forward.Rotate( transform.rotation * DegToRad );

	SetVelocity(forward * speed);

	// Bullets move at a constant speed.
	SetDamping(0.0f);

	// Destroy the bullet automatically.
	SetLifespan(lifespan);

	SetCollisionRadius(1.5f);
}