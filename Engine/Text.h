#pragma once

#include "Font.h"
#include "Renderer.h"
#include "Resource.h"
#include "Vector3.h"

#include <string>

struct SDL_Texture;

namespace nu {
	class Text {
	public:
		Text() = default;

		Text(res_t<Font> font) :
			a_font{ font }
		{}

		~Text();

		void SetFont(res_t<Font> font) {
			a_font = font;
		}

		bool Create(
			const Renderer& renderer,
			const std::string& text,
			const Color& color
		);

		void Draw(
			const Renderer& renderer,
			float x,
			float y
		);

	private:
		res_t<Font> a_font;
		SDL_Texture* a_texture{ nullptr };
	};
}