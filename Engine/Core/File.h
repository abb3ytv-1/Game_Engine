#pragma once

#include <string>

namespace nu {
	// Attempts to locate and set the working directory to the given path name.
	// The function searches the current working directory and its parents for
	// a matching child path and, if found, sets the process working directory
	// to that path and returns true. Returns false if not found or on error.
	bool SetWorkingDirectory(const std::string& pathName);

	// Returns the current working directory as a string.
	std::string GetWorkingDirectory();

	// Read the entire text contents of a file into 'out'. Returns true on success.
	bool ReadTextFile(const std::string& path, std::string& out);

	// Write text to a file. If append is true, append to the file; otherwise overwrite.
	// Returns true on success.
	bool WriteTextFile(const std::string& path, const std::string& contents, bool append = false);
}
