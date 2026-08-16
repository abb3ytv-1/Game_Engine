#include "pch.h"
#include "JsonSimple.h"

#include <fstream>
#include <sstream>
#include <cctype>
#include <iostream>
#include <vector>

using namespace nu;

namespace json {

static inline void trim(std::string &s) {
	size_t a = 0;
	while (a < s.size() && std::isspace((unsigned char)s[a])) ++a;
	size_t b = s.size();
	while (b > a && std::isspace((unsigned char)s[b-1])) --b;
	if (a == 0 && b == s.size()) return;
	s = s.substr(a, b - a);
}

static inline size_t skipSpaces(const std::string &s, size_t i) {
	while (i < s.size() && std::isspace((unsigned char)s[i])) ++i;
	return i;
}

static bool extractQuoted(const std::string &s, size_t pos, std::string &out, size_t &newPos) {
	if (pos >= s.size() || s[pos] != '"') return false;
	++pos;
	std::ostringstream ss;
	while (pos < s.size()) {
		char c = s[pos++];
		if (c == '\\') {
			if (pos >= s.size()) return false;
			char esc = s[pos++];
			switch (esc) {
			case '"': ss << '"'; break;
			case '\\': ss << '\\'; break;
			case '/': ss << '/'; break;
			case 'b': ss << '\b'; break;
			case 'f': ss << '\f'; break;
			case 'n': ss << '\n'; break;
			case 'r': ss << '\r'; break;
			case 't': ss << '\t'; break;
			default: ss << esc; break;
			}
		}
		else if (c == '"') {
			out = ss.str();
			newPos = pos;
			return true;
		}
		else {
			ss << c;
		}
	}
	return false;
}

static bool extractNumber(const std::string &s, size_t pos, double &out, size_t &newPos) {
	pos = skipSpaces(s, pos);
	size_t start = pos;
	if (pos < s.size() && (s[pos] == '-' || s[pos] == '+')) ++pos;
	bool hasDigits = false;
	while (pos < s.size() && std::isdigit((unsigned char)s[pos])) { ++pos; hasDigits = true; }
	if (pos < s.size() && s[pos] == '.') { ++pos; while (pos < s.size() && std::isdigit((unsigned char)s[pos])) { ++pos; hasDigits = true; } }
	if (!hasDigits) return false;
	try { out = std::stod(s.substr(start, pos - start)); }
	catch (...) { return false; }
	newPos = pos;
	return true;
}

static bool extractBool(const std::string &s, size_t pos, bool &out, size_t &newPos) {
	pos = skipSpaces(s, pos);
	if (s.compare(pos, 4, "true") == 0) { out = true; newPos = pos + 4; return true; }
	if (s.compare(pos, 5, "false") == 0) { out = false; newPos = pos + 5; return true; }
	return false;
}

static size_t findKeyColon(const std::string &data, const std::string &key) {
	std::string pattern = '"' + key + '"';
	size_t pos = 0;
	while (true) {
		pos = data.find(pattern, pos);
		if (pos == std::string::npos) return std::string::npos;
		size_t p = pos + pattern.size();
		p = skipSpaces(data, p);
		if (p < data.size() && data[p] == ':') return p;
		pos = p;
	}
}

bool Load(const std::string& filename, Document& doc) {
	std::ifstream f(filename);
	if (!f.is_open()) return false;
	std::stringstream ss; ss << f.rdbuf();
	std::string data = ss.str();

	std::vector<std::string> stringKeys = { "name" };
	for (auto &k : stringKeys) {
		size_t c = findKeyColon(data, k);
		if (c != std::string::npos) {
			size_t q = data.find('"', c);
			std::string val;
			size_t np;
			if (q != std::string::npos && extractQuoted(data, q, val, np)) {
				doc.strings[k] = val;
			}
		}
	}

	std::vector<std::string> numberKeys = { "age", "speed" };
	for (auto &k : numberKeys) {
		size_t c = findKeyColon(data, k);
		if (c != std::string::npos) {
			double v; size_t np;
			if (extractNumber(data, c + 1, v, np)) doc.numbers[k] = v;
		}
	}

	size_t cb = findKeyColon(data, "active");
	if (cb != std::string::npos) {
		bool bv; size_t np;
		if (extractBool(data, cb + 1, bv, np)) doc.bools["active"] = bv;
	}


	size_t cp = findKeyColon(data, "position");
	if (cp != std::string::npos) {
		size_t o = data.find('{', cp);
		if (o != std::string::npos) {
			size_t depth = 1; size_t i = o + 1;
			size_t end = std::string::npos;
			for (; i < data.size(); ++i) {
				if (data[i] == '{') ++depth;
				else if (data[i] == '}') { if (--depth == 0) { end = i; break; } }
			}
			if (end != std::string::npos) {
				std::string sub = data.substr(o + 1, end - o - 1);
				double vx=0, vy=0; size_t np;
				size_t kx = findKeyColon(sub, "x");
				if (kx != std::string::npos && extractNumber(sub, kx + 1, vx, np)) {}
				size_t ky = findKeyColon(sub, "y");
				if (ky != std::string::npos && extractNumber(sub, ky + 1, vy, np)) {}
				doc.vec2s["position"] = Vector2{ (float)vx, (float)vy };
			}
		}
	}

	size_t cc = findKeyColon(data, "color");
	if (cc != std::string::npos) {
		size_t o = data.find('{', cc);
		if (o != std::string::npos) {
			size_t depth = 1; size_t i = o + 1; size_t end = std::string::npos;
			for (; i < data.size(); ++i) { if (data[i] == '{') ++depth; else if (data[i] == '}') { if (--depth == 0) { end = i; break; } } }
			if (end != std::string::npos) {
				std::string sub = data.substr(o + 1, end - o - 1);
				double rx=0, ry=0, rz=0; size_t np;
				size_t kx = findKeyColon(sub, "x"); if (kx != std::string::npos) extractNumber(sub, kx + 1, rx, np);
				size_t ky = findKeyColon(sub, "y"); if (ky != std::string::npos) extractNumber(sub, ky + 1, ry, np);
				size_t kz = findKeyColon(sub, "z"); if (kz != std::string::npos) extractNumber(sub, kz + 1, rz, np);
				doc.vec3s["color"] = Vector3{ (float)rx, (float)ry, (float)rz };
			}
		}
	}

	return true;
}

bool Read(const Document& doc, const std::string& key, std::string& outValue) {
	auto it = doc.strings.find(key);
	if (it == doc.strings.end()) return false;
	outValue = it->second; return true;
}

bool Read(const Document& doc, const std::string& key, int& outValue) {
	auto it = doc.numbers.find(key);
	if (it == doc.numbers.end()) return false;
	outValue = static_cast<int>(it->second); return true;
}

bool Read(const Document& doc, const std::string& key, float& outValue) {
	auto it = doc.numbers.find(key);
	if (it == doc.numbers.end()) return false;
	outValue = static_cast<float>(it->second); return true;
}

bool Read(const Document& doc, const std::string& key, bool& outValue) {
	auto it = doc.bools.find(key);
	if (it == doc.bools.end()) return false;
	outValue = it->second; return true;
}

bool Read(const Document& doc, const std::string& key, nu::Vector2& outValue) {
	auto it = doc.vec2s.find(key);
	if (it == doc.vec2s.end()) return false;
	outValue = it->second; return true;
}

bool Read(const Document& doc, const std::string& key, nu::Vector3& outValue) {
	auto it = doc.vec3s.find(key);
	if (it == doc.vec3s.end()) return false;
	outValue = it->second; return true;
}

} // namespace json
