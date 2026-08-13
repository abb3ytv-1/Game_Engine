#include "pch.h"
#include "Json.h"
#include "rapidjson/filereadstream.h"
#include <cstdio>
#include <iostream>

namespace json
{
    bool Load(const std::string& filename, rapidjson::Document& document)
    {
        FILE* filePtr = nullptr;
        fopen_s(&filePtr, filename.c_str(), "rb");
        if (filePtr == nullptr)
        {
            std::cout << "Json::Load - failed to open file: " << filename << std::endl;
            return false;
        }

        char buffer[65536];
        rapidjson::FileReadStream stream(filePtr, buffer, sizeof(buffer));
        document.ParseStream(stream);
        fclose(filePtr);

        if (document.HasParseError())
        {
            std::cout << "Json::Load - parse error in file: " << filename << std::endl;
            return false;
        }
        return true;
    }

    // Macro to cut down repetition across the simple, single-value Read overloads.
    // TypeCheck / GetFunc are the rapidjson::Value member function names, e.g. IsInt / GetInt.
#define JSON_READ_MEMBER(TypeCheck, GetFunc)               \
        if (!value.HasMember(key.c_str())) return false;       \
        const rapidjson::Value& member = value[key.c_str()];   \
        if (!member.TypeCheck()) return false;                 \
        outValue = member.GetFunc();                           \
        return true;

    bool Read(const rapidjson::Value& value, const std::string& key, int& outValue)
    {
        JSON_READ_MEMBER(IsInt, GetInt)
    }

    bool Read(const rapidjson::Value& value, const std::string& key, bool& outValue)
    {
        JSON_READ_MEMBER(IsBool, GetBool)
    }

    bool Read(const rapidjson::Value& value, const std::string& key, float& outValue)
    {
        JSON_READ_MEMBER(IsNumber, GetFloat)
    }

    bool Read(const rapidjson::Value& value, const std::string& key, std::string& outValue)
    {
        JSON_READ_MEMBER(IsString, GetString)
    }

#undef JSON_READ_MEMBER

    bool Read(const rapidjson::Value& value, const std::string& key, nu::Vector2& outValue)
    {
        if (!value.HasMember(key.c_str())) return false;
        const rapidjson::Value& member = value[key.c_str()];
        if (!member.IsArray() || member.Size() != 2) return false;
        if (!member[0].IsNumber() || !member[1].IsNumber()) return false;

        outValue.x = member[0].GetFloat();
        outValue.y = member[1].GetFloat();
        return true;
    }

    bool Read(const rapidjson::Value& value, const std::string& key, nu::Vector3& outValue)
    {
        if (!value.HasMember(key.c_str())) return false;
        const rapidjson::Value& member = value[key.c_str()];
        if (!member.IsArray() || member.Size() != 3) return false;

        for (rapidjson::SizeType i = 0; i < 3; ++i)
        {
            if (!member[i].IsNumber()) return false;
        }

        outValue.x = member[0].GetFloat();
        outValue.y = member[1].GetFloat();
        outValue.z = member[2].GetFloat();
        return true;
    }
}