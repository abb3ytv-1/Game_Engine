#include "pch.h"
#include "Factory.h"
#include "../Framework/Object.h"
#include "../Framework/Actor.h"

namespace nu {

	Factory& Factory::Instance() {
		static Factory instance;
		return instance;
	}

	bool Factory::Register(const std::string& name, std::unique_ptr<ICreator> creator) {
		if (a_creators.find(name) != a_creators.end()) {
			return false;
		}

		a_creators[name] = std::move(creator);
		return true;
	}

	std::unique_ptr<Object> Factory::Create(const std::string& name) const {
		auto it = a_creators.find(name);
		if (it == a_creators.end()) {
			return nullptr;
		}

		return it->second->Create();
	}

	std::unique_ptr<Actor> Factory::CreateActor(const std::string& name) const {
		std::unique_ptr<Object> base = Create(name);
		if (!base) {
			return nullptr;
		}

		// Safely check whether the created Object is an Actor without releasing ownership
		Actor* raw = dynamic_cast<Actor*>(base.get());
		if (raw == nullptr) {
			// created object is not an Actor - keep base owned and return nullptr
			return nullptr;
		}

		// Ownership transfer: release base and return Actor unique_ptr
		base.release();
		return std::unique_ptr<Actor>(raw);
	}

}
