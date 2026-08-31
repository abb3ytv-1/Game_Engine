#pragma once

#include "../Math/Transform.h"
#include "../Renderer/Model.h"
#include "Object.h"

#include <memory>
#include <iostream>
#include <vector>
#include <type_traits>
#include "Component.h"
#include "../Physics/RigidBodyComponent.h"
#include "../Physics/CircleCollider2DComponent.h"

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

		void Destroy() {
			a_destroyed = true;
		}

		bool IsDestroyed() const {
			return a_destroyed;
		}

		// Collision
		void SetCollisionRadius(float radius)
		{
			auto* collider = GetComponent<CircleCollider2DComponent>();

			if (!collider)
			{
				AddComponent(
					std::make_unique<CircleCollider2DComponent>(radius)
				);
			}
			else
			{
				collider->SetRadius(radius);
			}
		}

		float GetCollisionRadius() const
		{
			auto* collider =
				const_cast<Actor*>(this)
				->GetComponent<CircleCollider2DComponent>();

			if (collider)
			{
				return collider->GetRadius() * a_transform.scale;
			}

			return 0.0f;
		}

		bool IsColliding(const Actor& other) const;

		void SetTag(const std::string& tag) {
			a_tag = tag;
		}

		const std::string& GetTag() const {
			return a_tag;
		}

	protected:
		Transform a_transform;

		float a_lifespan{ -1.0f };
		std::string a_tag;

public:
		bool a_destroyed{ false };

		Model a_model;

		std::vector<std::unique_ptr<Component>> a_components;
	};
}