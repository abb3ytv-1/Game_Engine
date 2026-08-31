#pragma once

#include <map>
#include <memory>
#include <string>

namespace nu {
	class Object;
	class Actor;

	class ICreator {
	public:
		virtual ~ICreator() = default;
		virtual std::unique_ptr<Object> Create() = 0;
	};

	template<typename T>
	class Creator : public ICreator {
	public:
		std::unique_ptr<Object> Create() override {
			return std::make_unique<T>();
		}
	};
	
	class Factory {
	public:
		static Factory& Instance();

		bool Register(const std::string& name, std::unique_ptr<ICreator> creator);

		// Create as base Object
		std::unique_ptr<Object> Create(const std::string& name) const;

		// Helper to create Actor instances (returns nullptr if created object is not an Actor)
		std::unique_ptr<Actor> CreateActor(const std::string& name) const;

	private:
		Factory() = default;
		std::map<std::string, std::unique_ptr<ICreator>> a_creators;
	};
}
