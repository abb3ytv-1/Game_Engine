#include "pch.h"
#include "File.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace nu {
	std::string GetWorkingDirectory() {
		std::error_code ec;
		std::filesystem::path path =
			std::filesystem::current_path(ec);

		if (ec) {
			std::cerr << ec.message() << '\n';
			return {};
		}

		return path.string();
	}

	bool SetWorkingDirectory(const std::string& path) {
		std::error_code ec;

		std::filesystem::current_path(path, ec);

		if (ec) {
			std::cerr << ec.message() << '\n';
		}

		return !ec;
	}

	std::string GetFilename(const std::string& path) {
		return std::filesystem::path{ path }
			.filename()
			.string();
	}

	std::string GetFileExtension(const std::string& path) {
		return std::filesystem::path{ path }
			.extension()
			.string();
	}

	std::string GetFilenameNoExtension(const std::string& path) {
		return std::filesystem::path{ path }
			.stem()
			.string();
	}

	bool FileExists(const std::string& path) {
		std::error_code ec;

		bool result = std::filesystem::exists(path, ec);

		if (ec) {
			std::cerr << ec.message() << '\n';
		}

		return !ec && result;
	}

	std::vector<std::string> GetFilesInDirectory(
		const std::string& path
	) {
		std::vector<std::string> files;
		std::error_code ec;

		std::filesystem::directory_iterator iterator{ path, ec };

		if (ec) {
			std::cerr << ec.message() << '\n';
			return files;
		}

		for (const auto& entry : iterator) {
			if (entry.is_regular_file(ec) && !ec) {
				files.push_back(entry.path().string());
			}
		}

		return files;
	}

	std::vector<std::string> GetDirectoriesIn(
		const std::string& path
	) {
		std::vector<std::string> directories;
		std::error_code ec;

		std::filesystem::directory_iterator iterator{ path, ec };

		if (ec) {
			std::cerr << ec.message() << '\n';
			return directories;
		}

		for (const auto& entry : iterator) {
			if (entry.is_directory(ec) && !ec) {
				directories.push_back(entry.path().string());
			}
		}

		return directories;
	}

	bool ReadTextFile(
		const std::string& path,
		std::string& data
	) {
		std::ifstream file{ path };

		if (!file.is_open()) {
			return false;
		}

		std::stringstream stream;
		stream << file.rdbuf();

		data = stream.str();

		return true;
	}

	bool WriteTextFile(
		const std::string& path,
		const std::string& data,
		bool append
	) {
		std::ios::openmode mode =
			append ? std::ios::app : std::ios::out;

		std::ofstream file{ path, mode };

		if (!file.is_open()) {
			return false;
		}

		file << data;

		return true;
	}
}