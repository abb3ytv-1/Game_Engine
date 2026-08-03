#pragma once

#include <string>

struct TTF_Font;

namespace nu {
	class Font {
		friend class Text;

	public:
		Font() = default;
		~Font();

		bool Load(
			const std::string& filename,
			float fontSize
		);

	private:
		TTF_Font* a_ttfFont{ nullptr };
	};
}