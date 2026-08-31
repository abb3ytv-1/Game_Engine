#pragma once

#include "./Resources/Resource.h"

#include <SDL3/SDL.h>

#include <string>
#include <vector>

namespace nu {

	class TextureFrames : public Resource {
	public:
		TextureFrames() = default;

		bool Load(
			const std::string& name,
			int textureWidth,
			int textureHeight,
			int frameWidth,
			int frameHeight
		);

		const SDL_FRect& GetFrame(std::size_t index) const;

		std::size_t GetFrameCount() const {
			return a_frames.size();
		}

	private:
		std::vector<SDL_FRect> a_frames;
	};

}