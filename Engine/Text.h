#pragma once

#include "Font.h"
#include "Renderer.h"
#include "Vector3.h"

#include <string>

struct SDL_Texture;

namespace nu {
	class Text {
	public:
		Text() = default;

		Text(Font* font) :
			a_font{ font }
		{}

		~Text();

		Text(const Text&) = delete;
		Text& operator=(const Text&) = delete;

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

		void SetFont(Font* font) {
			a_font = font;
		}

	private:
		// Text uses the font but does not own it.
		Font* a_font{ nullptr };

		SDL_Texture* a_texture{ nullptr };
	};
}