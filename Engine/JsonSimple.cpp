#include "pch.h"
#include "JsonSimple.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace nu::json
{
	bool Load(
		const std::string& filename,
		rapidjson::Document& document
	)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			std::cerr << "Could not open JSON file: "
				<< filename << std::endl;

			return false;
		}

		std::stringstream buffer;
		buffer << file.rdbuf();

		std::stringstream stream(buffer.str());
		rapidjson::IStreamWrapper istream(stream);

		document.ParseStream(istream);

		if (document.HasParseError())
		{
			std::cerr << "Could not parse JSON: "
				<< filename << std::endl;

			std::cerr << "Error: "
				<< rapidjson::GetParseError_En(
					document.GetParseError()
				)
				<< std::endl;

			return false;
		}

		if (!document.IsObject())
		{
			std::cerr << "JSON root is not an object: "
				<< filename << std::endl;

			return false;
		}

		std::cout << "Loaded JSON: "
			<< filename << std::endl;

		return true;
	}


	bool Read(
		const rapidjson::Value& value,
		const std::string& name,
		int& data
	)
	{
		if (!value.HasMember(name.c_str()) ||
			!value[name.c_str()].IsInt())
		{
			std::cerr << "Could not read JSON value (int): "
				<< name << std::endl;

			return false;
		}

		data = value[name.c_str()].GetInt();

		return true;
	}


	bool Read(
		const rapidjson::Value& value,
		const std::string& name,
		float& data
	)
	{
		if (!value.HasMember(name.c_str()) ||
			!value[name.c_str()].IsNumber())
		{
			std::cerr << "Could not read JSON value (float): "
				<< name << std::endl;

			return false;
		}

		data = value[name.c_str()].GetFloat();

		return true;
	}


	bool Read(
		const rapidjson::Value& value,
		const std::string& name,
		bool& data
	)
	{
		if (!value.HasMember(name.c_str()) ||
			!value[name.c_str()].IsBool())
		{
			std::cerr << "Could not read JSON value (bool): "
				<< name << std::endl;

			return false;
		}

		data = value[name.c_str()].GetBool();

		return true;
	}


	bool Read(
		const rapidjson::Value& value,
		const std::string& name,
		std::string& data
	)
	{
		if (!value.HasMember(name.c_str()) ||
			!value[name.c_str()].IsString())
		{
			std::cerr << "Could not read JSON value (string): "
				<< name << std::endl;

			return false;
		}

		data = value[name.c_str()].GetString();

		return true;
	}


	bool Read(
		const rapidjson::Value& value,
		const std::string& name,
		Vector2& data
	)
	{
		if (!value.HasMember(name.c_str()) ||
			!value[name.c_str()].IsArray() ||
			value[name.c_str()].Size() != 2)
		{
			std::cerr << "Could not read JSON value (Vector2): "
				<< name << std::endl;

			return false;
		}

		const auto& array = value[name.c_str()];

		for (rapidjson::SizeType i = 0; i < array.Size(); ++i)
		{
			if (!array[i].IsNumber())
			{
				std::cerr << "Could not read JSON value (Vector2): "
					<< name << std::endl;

				return false;
			}

			data[i] = array[i].GetFloat();
		}

		return true;
	}
}