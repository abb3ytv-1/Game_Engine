#pragma once

#include "Component.h"
#include <memory>
#include "Vector2.h"

namespace nu {
	class Texture;
	class Renderer;

	class SpriteRendererComponent : public Component {
	public:
		SpriteRendererComponent() = default;

		SpriteRendererComponent(
			const std::shared_ptr<Texture>& tex,
			float scale = 1.0f,
			const Vector2& origin = Vector2{ 0.5f, 0.5f }
		)
			: a_texture(tex),
			a_scale(scale),
			a_origin(origin)
		{}

		std::unique_ptr<Component> Clone() const override;

		void Draw(
			const Renderer& renderer,
			const Actor& actor
		) const override;

	private:
		std::shared_ptr<Texture> a_texture;
		float a_scale{ 1.0f };
		Vector2 a_origin{ 0.5f, 0.5f };
	};
}