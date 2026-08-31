#include "pch.h"
#include "TextureFrames.h"

namespace nu {

	bool TextureFrames::Load(
		const std::string& name,
		int textureWidth,
		int textureHeight,
		int frameWidth,
		int frameHeight
	) {
		(void)name;

		a_frames.clear();

		if (
			textureWidth <= 0 ||
			textureHeight <= 0 ||
			frameWidth <= 0 ||
			frameHeight <= 0
			) {
			return false;
		}

		int columns = textureWidth / frameWidth;
		int rows = textureHeight / frameHeight;

		if (columns <= 0 || rows <= 0) {
			return false;
		}

		for (int y = 0; y < rows; ++y) {
			for (int x = 0; x < columns; ++x) {
				a_frames.push_back(
					SDL_FRect{
						static_cast<float>(x * frameWidth),
						static_cast<float>(y * frameHeight),
						static_cast<float>(frameWidth),
						static_cast<float>(frameHeight)
					}
				);
			}
		}

		return !a_frames.empty();
	}

	const SDL_FRect& TextureFrames::GetFrame(
		std::size_t index
	) const {
		return a_frames.at(index);
	}

}