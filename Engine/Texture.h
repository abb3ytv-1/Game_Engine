#pragma once

#include "Vector2.h"
#include "Resource.h"

#include <string>

struct SDL_Texture;

namespace nu {
	class Renderer;

	class Texture : public Resource {
	public:
		Texture() = default;
		~Texture() override;

		// Prevent two Texture objects from owning the same SDL_Texture.
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		bool Load(
			const std::string& filename,
			Renderer& renderer
		);

		Vector2 GetSize() const;

	private:
		SDL_Texture* a_texture{ nullptr };

		friend class Renderer;
	};
}