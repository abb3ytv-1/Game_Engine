#pragma once

#include "Transform.h"
#include "Model.h"
#include "Object.h"

#include <memory>
#include <iostream>
#include <vector>
#include <type_traits>
#include "Component.h"
#include "RigidBodyComponent.h"

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

		// Component support
		void AddComponent(std::unique_ptr<Component> comp);

		template<typename T>
		T* GetComponent() {
			for (auto& c : a_components) {
				T* t = dynamic_cast<T*>(c.get());
				if (t) return t;
			}
			return nullptr;
		}

		// Deep-copy support for components
		Actor(const Actor& other);
		Actor& operator=(const Actor& other);

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

		// Velocity and damping are managed by RigidBodyComponent when present.
		Vector2 GetVelocity() const {
			auto* rb = const_cast<Actor*>(this)->GetComponent<nu::RigidBodyComponent>();
			if (rb) return rb->GetVelocity();
			return Vector2{0.0f, 0.0f};
		}

		void SetVelocity(const Vector2& velocity) {
			auto* rb = GetComponent<nu::RigidBodyComponent>();
			if (!rb) {
				auto rbComp = std::make_unique<nu::RigidBodyComponent>();
				rbComp->SetVelocity(velocity);
				AddComponent(std::move(rbComp));
			} else {
				rb->SetVelocity(velocity);
			}
		}

		float GetDamping() const {
			auto* rb = const_cast<Actor*>(this)->GetComponent<nu::RigidBodyComponent>();
			if (rb) return rb->GetDamping();
			return 1.0f;
		}

		void SetDamping(float damping) {
			auto* rb = GetComponent<nu::RigidBodyComponent>();
			if (!rb) {
				auto rbComp = std::make_unique<nu::RigidBodyComponent>();
				rbComp->SetDamping(damping);
				AddComponent(std::move(rbComp));
			} else {
				rb->SetDamping(damping);
			}
		}

		float GetLifespan() const {
			return a_lifespan;
		}

		void SetLifespan(float lifespan) {
			a_lifespan = lifespan;
		}

		// Sprite texture is managed by SpriteRendererComponent

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

		// Sprite handled by SpriteRendererComponent now

public:
		// Texture origin is managed by SpriteRendererComponent

		// Radius before the actor's scale is applied
		float a_collisionRadius{ 1.0f };

		bool a_destroyed{ false };

		Model a_model;

		// Components owned by this actor
		std::vector<std::unique_ptr<Component>> a_components;
	};
}