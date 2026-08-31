#include "pch.h"
#include "Font.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

using namespace nu;

Font::~Font() {
	if (a_ttfFont != nullptr) {
		TTF_CloseFont(a_ttfFont);
		a_ttfFont = nullptr;
	}
}

bool Font::Load(
	const std::string& filename,
	float fontSize
) {
	if (a_ttfFont != nullptr) {
		TTF_CloseFont(a_ttfFont);
		a_ttfFont = nullptr;
	}

	a_ttfFont = TTF_OpenFont(
		filename.c_str(),
		fontSize
	);

	if (a_ttfFont == nullptr) {
		std::cerr
			<< "Could not load font: "
			<< filename
			<< '\n'
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	return true;
}