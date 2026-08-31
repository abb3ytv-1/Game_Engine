#include "pch.h"
#include "SpriteAnimationRendererComponent.h"

#include "./Framework/Actor.h"

#include "./Renderer/Renderer.h"
#include "./Renderer/Texture.h"
#include "TextureFrames.h"

namespace nu {

	std::unique_ptr<Component>
		SpriteAnimationRendererComponent::Clone() const
	{
		auto component =
			std::make_unique<SpriteAnimationRendererComponent>(
				a_texture,
				a_frames,
				a_scale,
				a_origin,
				a_frameRate
			);

		component->a_frameTimer = a_frameTimer;
		component->a_currentFrame = a_currentFrame;
		component->a_playing = a_playing;
		component->a_looping = a_looping;

		return component;
	}

	void SpriteAnimationRendererComponent::Update(
		Actor& actor,
		float dt
	) {
		(void)actor;

		if (!a_playing ||
			!a_frames ||
			a_frames->GetFrameCount() == 0 ||
			a_frameRate <= 0.0f) {
			return;
		}

		a_frameTimer += dt;

		float frameDuration = 1.0f / a_frameRate;

		while (a_frameTimer >= frameDuration) {
			a_frameTimer -= frameDuration;

			++a_currentFrame;

			if (a_currentFrame >= a_frames->GetFrameCount()) {
				if (a_looping) {
					a_currentFrame = 0;
				}
				else {
					a_currentFrame =
						a_frames->GetFrameCount() - 1;

					a_playing = false;
				}
			}
		}
	}

	void SpriteAnimationRendererComponent::Draw(
		const Renderer& renderer,
		const Actor& actor
	) const {
		if (!a_texture ||
			!a_frames ||
			a_frames->GetFrameCount() == 0) {
			return;
		}

		const SDL_FRect& frame =
			a_frames->GetFrame(a_currentFrame);

		renderer.DrawTexture(
			*a_texture,
			actor.GetTransform(),
			frame,
			a_scale,
			a_origin
		);
	}

}