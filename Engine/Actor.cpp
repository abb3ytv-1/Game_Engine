#include "pch.h"
#include "Actor.h"
#include "Renderer.h"
#include "MathUtils.h"
#include "Component.h"
#include "CircleCollider2DComponent.h"

namespace nu {
	void Actor::Update(float dt) {
		if (a_destroyed) {
			return;
		}

		if (a_lifespan > 0.0f) {
			a_lifespan -= dt;

			if (a_lifespan <= 0.0f) {
				Destroy();
				return;
			}
		}


		a_transform.position.x = Wrap(
			0.0f,
			1920.0f,
			a_transform.position.x
		);

		a_transform.position.y = Wrap(
			0.0f,
			1080.0f,
			a_transform.position.y
		);

		for (auto& comp : a_components) {
			if (comp) comp->Update(*this, dt);
		}
	}

	void Actor::Draw(
		const Renderer& renderer
	) const {
		if (a_destroyed) {
			return;
		}

		if (!a_model.GetMeshes().empty()) {
			renderer.DrawModel(
				a_model,
				a_transform
			);
		}

		for (auto& comp : a_components) {
			if (comp) comp->Draw(renderer, *this);
		}
	}

	void Actor::AddComponent(std::unique_ptr<Component> comp) {
		if (!comp) return;
		comp->OnAttach(*this);
		a_components.push_back(std::move(comp));
	}

	Actor::Actor(const Actor& other)
	{
		a_transform = other.a_transform;
		a_lifespan = other.a_lifespan;
		a_destroyed = other.a_destroyed;
		a_model = other.a_model;

		for (auto& comp : other.a_components)
		{
			if (comp)
			{
				a_components.push_back(comp->Clone());
			}
		}
	}

	Actor& Actor::operator=(const Actor& other)
	{
		if (this == &other)
		{
			return *this;
		}

		a_transform = other.a_transform;
		a_lifespan = other.a_lifespan;
		a_destroyed = other.a_destroyed;
		a_model = other.a_model;

		a_components.clear();

		for (auto& comp : other.a_components)
		{
			if (comp)
			{
				a_components.push_back(comp->Clone());
			}
		}

		return *this;
	}

	bool Actor::IsColliding(const Actor& other) const
	{
		auto* collider =
			const_cast<Actor*>(this)
			->GetComponent<CircleCollider2DComponent>();

		auto* otherCollider =
			const_cast<Actor&>(other)
			.GetComponent<CircleCollider2DComponent>();

		if (!collider || !otherCollider)
		{
			return false;
		}

		return collider->IsColliding(
			*this,
			*otherCollider,
			other
		);
	}
}
