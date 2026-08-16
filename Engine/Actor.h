#pragma once

#include "Transform.h"
#include "Model.h"
#include "Object.h"

#include <memory>
#include <iostream>

namespace nu {
	class Texture;

	class Actor : public Object {
	public:
		Actor() = default;
		virtual ~Actor() = default;

		Actor(const Transform& transform) :
			a_transform{ transform }
		{}

		Actor(
			const Transform& transform,
			const Model& model
		) :
			a_transform{ transform },
			a_model{ model }
		{}

		virtual void Update(float dt);

		virtual void Draw(
			const class Renderer& renderer
		) const;

		const Transform& GetTransform() const {
			return a_transform;
		}

		void SetTransform(const Transform& transform) {
			a_transform = transform;
		}

		void SetPosition(const Vector2& position) {
			a_transform.position = position;
		}

		void SetRotation(float rotation) {
			a_transform.rotation = rotation;
		}

		void SetScale(float scale) {
			a_transform.scale = scale;
		}

		const Vector2& GetVelocity() const {
			return a_velocity;
		}

		void SetVelocity(const Vector2& velocity) {
			a_velocity = velocity;
		}

		float GetDamping() const {
			return a_damping;
		}

		void SetDamping(float damping) {
			a_damping = damping;
		}

		float GetLifespan() const {
			return a_lifespan;
		}

		void SetLifespan(float lifespan) {
			a_lifespan = lifespan;
		}

		// Sprite texture
		void SetTexture(
			const std::shared_ptr<Texture>& texture
		) {
#ifdef _DEBUG
			// Diagnostic output to help trace crashes during development
			std::cerr << "Actor::SetTexture this=" << static_cast<const void*>(this)
				<< " incoming=" << static_cast<const void*>(texture.get()) << "\n";
#endif
			a_texture = texture;
		}

		const std::shared_ptr<Texture>& GetTexture() const {
			return a_texture;
		}

		void SetTextureScale(float scale) {
			a_textureScale = scale;
		}

		float GetTextureScale() const {
			return a_textureScale;
		}

		// Collision
		void SetCollisionRadius(float radius) {
			a_collisionRadius = radius;
		}

		float GetCollisionRadius() const {
			return (
				a_collisionRadius *
				a_transform.scale
				);
		}

		bool IsColliding(const Actor& other) const;

		bool IsDestroyed() const {
			return a_destroyed;
		}

		void Destroy() {
			a_destroyed = true;
		}

	protected:
		Transform a_transform;
		Vector2 a_velocity{ 0.0f, 0.0f };

		float a_damping{ 1.0f };
		float a_lifespan{ -1.0f };

		// Sprite
		std::shared_ptr<Texture> a_texture;
		float a_textureScale{ 1.0f };

		// Texture origin (normalized 0..1 where {0.5,0.5} is center)
		Vector2 a_textureOrigin{ 0.5f, 0.5f };

public:
		void SetTextureOrigin(const Vector2& origin) { a_textureOrigin = origin; }
		const Vector2& GetTextureOrigin() const { return a_textureOrigin; }

		// Radius before the actor's scale is applied
		float a_collisionRadius{ 1.0f };

		bool a_destroyed{ false };

		Model a_model;
	};
}