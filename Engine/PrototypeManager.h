#pragma once

#include "Actor.h"

#include <memory>
#include <string>

#include <rapidjson/document.h>

namespace nu
{
	class PrototypeManager
	{
	public:
		bool Load(const rapidjson::Value& prototypes);

		std::unique_ptr<Actor> Instantiate(
			const std::string& name
		) const;

		bool HasPrototype(
			const std::string& name
		) const;

	private:
		rapidjson::Document a_prototypes;
	};
}