#include "Vector2.h"
#include <iostream>

struct SDL_Texture;

namespace nu {
	class Texture
	{
	public:
		Texture() = default;
		~Texture();

		bool Load(const std::string& filename, class Renderer& renderer);

		Vector2 GetSize();

		friend class Renderer;

	private:
		SDL_Texture* a_texture{ nullptr };
	};
}