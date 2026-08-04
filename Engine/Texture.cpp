#include "pch.h"

#include "Texture.h"
#include "Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <iostream>

using namespace nu;

Texture::~Texture() {
	if (a_texture != nullptr) {
		SDL_DestroyTexture(a_texture);
		a_texture = nullptr;
	}
}

bool Texture::Load(
	const std::string& filename,
	Renderer& renderer
) {
	// Destroy the previous texture if this object is reused.
	if (a_texture != nullptr) {
		SDL_DestroyTexture(a_texture);
		a_texture = nullptr;
	}

	SDL_Surface* surface = IMG_Load(filename.c_str());

	if (surface == nullptr) {
		std::cerr
			<< "Could not load image: "
			<< filename
			<< "\nSDL error: "
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	a_texture = SDL_CreateTextureFromSurface(
		renderer.a_renderer,
		surface
	);

	SDL_DestroySurface(surface);

	if (a_texture == nullptr) {
		std::cerr
			<< "Could not create texture: "
			<< filename
			<< "\nSDL error: "
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	return true;
}

Vector2 Texture::GetSize() const {
	if (a_texture == nullptr) {
		return Vector2{ 0.0f, 0.0f };
	}

	Vector2 size{ 0.0f, 0.0f };

	if (!SDL_GetTextureSize(
		a_texture,
		&size.x,
		&size.y
	)) {
		std::cerr
			<< "Could not get texture size: "
			<< SDL_GetError()
			<< '\n';

		return Vector2{ 0.0f, 0.0f };
	}

	return size;
}