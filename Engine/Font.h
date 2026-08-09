#pragma once

#include "Resource.h"

#include <string>

struct TTF_Font;

namespace nu {
	class Text;

	class Font : public Resource {
		friend class Text;

	public:
		Font() = default;
		~Font() override;

		bool Load(
			const std::string& filename,
			float fontSize
		);

	private:
		TTF_Font* a_ttfFont{ nullptr };
	};
}