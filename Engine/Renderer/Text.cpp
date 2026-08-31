#include "pch.h"
#include "Text.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <cstdint>
#include <iostream>

using namespace nu;

namespace {
	std::uint8_t ToColorChannel(float value) {
		float clampedValue =
			std::clamp(value, 0.0f, 1.0f);

		return static_cast<std::uint8_t>( clampedValue * 255.0f );
	}
}

Text::~Text() {
	if (a_texture != nullptr) {
		SDL_DestroyTexture(a_texture);
		a_texture = nullptr;
	}
}

bool Text::Create( const Renderer& renderer, const std::string& text, const Color& color) {
	if (a_font == nullptr) {
		std::cerr
			<< "Text has no Font object.\n";

		return false;
	}

	if (a_font->a_ttfFont == nullptr) {
		std::cerr
			<< "Font object exists, but a_ttfFont is null.\n";

		return false;
	}

	if (a_texture != nullptr) {
		SDL_DestroyTexture(a_texture);
		a_texture = nullptr;
	}

	SDL_Color textColor{ 
		ToColorChannel(color.r),
		ToColorChannel(color.g),
		ToColorChannel(color.b),
		255
	};

	SDL_Surface* surface =
		TTF_RenderText_Solid( a_font->a_ttfFont, text.c_str(), text.size(), textColor );

	if (surface == nullptr) {
		std::cerr
			<< "Could not create text surface: "
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	a_texture =
		SDL_CreateTextureFromSurface( renderer.a_renderer, surface );

	SDL_DestroySurface(surface);

	if (a_texture == nullptr) {
		std::cerr
			<< "Could not create text texture: "
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	return true;
}

void Text::Draw( const Renderer& renderer, float x, float y ) {
	if (a_texture == nullptr) {
		return;
	}

	float width = 0.0f;
	float height = 0.0f;

	if (!SDL_GetTextureSize( a_texture, &width, &height )) {
		std::cerr
			<< "Could not get text texture size: "
			<< SDL_GetError()
			<< '\n';

		return;
	}

	SDL_FRect destination{
		x,
		y,
		width,
		height
	};

	if (!SDL_RenderTexture( renderer.a_renderer, a_texture, nullptr, &destination )) {
		std::cerr
			<< "Could not draw text: "
			<< SDL_GetError()
			<< '\n';
	}
}