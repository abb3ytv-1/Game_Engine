#pragma#pragma once
#include "pch.h"
#include "rapidjson/document.h"
#include "Vector2.h"
#include "Vector3.h"
#include <string>

namespace json
{
    bool Load(const std::string& filename, rapidjson::Document& document);

    bool Read(const rapidjson::Value& value, const std::string& key, int& outValue);
    bool Read(const rapidjson::Value& value, const std::string& key, bool& outValue);
    bool Read(const rapidjson::Value& value, const std::string& key, float& outValue);
    bool Read(const rapidjson::Value& value, const std::string& key, std::string& outValue);
    bool Read(const rapidjson::Value& value, const std::string& key, nu::Vector2& outValue);
    bool Read(const rapidjson::Value& value, const std::string& key, nu::Vector3& outValue);
}