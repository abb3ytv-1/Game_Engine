#include "pch.h"
#include "Serializer.h"

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>
#include <iostream>

namespace nu {

	bool Serializer::LoadJSON(
		const std::string& filename,
		rapidjson::Document& data
	) {
		std::ifstream file(filename);

		if (!file.is_open()) {
			std::cerr << "Failed to open JSON file: "
				<< filename << std::endl;
			return false;
		}

		rapidjson::IStreamWrapper stream(file);

		data.ParseStream(stream);

		if (data.HasParseError()) {
			std::cerr << "Failed to parse JSON file: "
				<< filename << std::endl;

			std::cerr << "Parse error code: "
				<< data.GetParseError() << std::endl;

			return false;
		}

		std::cout << "Loaded JSON file: "
			<< filename << std::endl;

		return true;
	}

}