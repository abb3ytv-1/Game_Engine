#include "pch.h"
#include "SpriteRendererComponent.h"
#include "Renderer.h"
#include "Actor.h"

namespace nu {

	std::unique_ptr<Component> SpriteRendererComponent::Clone() const
	{
		return std::make_unique<SpriteRendererComponent>(
			a_texture,
			a_scale,
			a_origin
		);
	}

	void SpriteRendererComponent::Draw(
		const Renderer& renderer,
		const Actor& actor
	) const
	{
		if (a_texture)
		{
			renderer.DrawTexture(
				*a_texture,
				actor.GetTransform(),
				a_scale,
				a_origin
			);
		}
	}
}