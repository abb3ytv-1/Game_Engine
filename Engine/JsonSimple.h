#pragma once

#include <string>
#include <map>
#include "Vector2.h"
#include "Vector3.h"

namespace json {

struct Document {
	std::map<std::string, std::string> strings;
	std::map<std::string, double> numbers;
	std::map<std::string, bool> bools;
	std::map<std::string, nu::Vector2> vec2s;
	std::map<std::string, nu::Vector3> vec3s;
};

bool Load(const std::string& filename, Document& doc);

bool Read(const Document& doc, const std::string& key, std::string& outValue);
bool Read(const Document& doc, const std::string& key, int& outValue);
bool Read(const Document& doc, const std::string& key, float& outValue);
bool Read(const Document& doc, const std::string& key, bool& outValue);
bool Read(const Document& doc, const std::string& key, nu::Vector2& outValue);
bool Read(const Document& doc, const std::string& key, nu::Vector3& outValue);

}
