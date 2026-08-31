#pragma once

#include <string>
#include <rapidjson/document.h>

namespace nu {

	class Serializer {
	public:
		static bool LoadJSON(
			const std::string& filename,
			rapidjson::Document& data
		);
	};

}