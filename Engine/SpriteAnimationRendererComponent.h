#pragma once

#include "./Framework/Component.h"

#include "./Math/Vector2.h"

#include <memory>

namespace nu {

	class Texture;
	class TextureFrames;
	class Renderer;
	class Actor;

	class SpriteAnimationRendererComponent : public Component {
	public:
		SpriteAnimationRendererComponent() = default;

		SpriteAnimationRendererComponent(
			const std::shared_ptr<Texture>& texture,
			const std::shared_ptr<TextureFrames>& frames,
			float scale = 1.0f,
			const Vector2& origin = Vector2{ 0.5f, 0.5f },
			float frameRate = 8.0f
		)
			: a_texture(texture),
			a_frames(frames),
			a_scale(scale),
			a_origin(origin),
			a_frameRate(frameRate)
		{}

		std::unique_ptr<Component> Clone() const override;

		void Update(
			Actor& actor,
			float dt
		) override;

		void Draw(
			const Renderer& renderer,
			const Actor& actor
		) const override;

	private:
		std::shared_ptr<Texture> a_texture;
		std::shared_ptr<TextureFrames> a_frames;

		float a_scale{ 1.0f };
		Vector2 a_origin{ 0.5f, 0.5f };

		float a_frameRate{ 8.0f };
		float a_frameTimer{ 0.0f };

		std::size_t a_currentFrame{ 0 };

		bool a_playing{ true };
		bool a_looping{ true };
	};

}