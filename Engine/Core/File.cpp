#include "pch.h"
#include "File.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace nu {

	bool SetWorkingDirectory(const std::string& pathName) {
		namespace fs = std::filesystem;

		fs::path p = fs::current_path();
		std::error_code ec;

		// Search current directory and parents for a child path matching pathName
		while (true) {
			fs::path candidate = p / pathName;
			if (fs::exists(candidate, ec) && !ec) {
				fs::current_path(candidate, ec);
				return !ec;
			}

			if (!p.has_parent_path()) break;
			p = p.parent_path();
		}

		// Not found
		return false;
	}

	std::string GetWorkingDirectory() {
		std::error_code ec;
		auto p = std::filesystem::current_path(ec);
		if (ec) return std::string();
		return p.string();
	}

	bool ReadTextFile(const std::string& path, std::string& out) {
		std::ifstream ifs(path, std::ios::in);
		if (!ifs) return false;
		std::ostringstream ss;
		ss << ifs.rdbuf();
		out = ss.str();
		return true;
	}

	bool WriteTextFile(const std::string& path, const std::string& contents, bool append) {
		std::ofstream ofs;
		if (append)
			ofs.open(path, std::ios::out | std::ios::app);
		else
			ofs.open(path, std::ios::out | std::ios::trunc);

		if (!ofs) return false;
		ofs << contents;
		return true;
	}

}
