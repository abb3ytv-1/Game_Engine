#pragma once

#include "Resource.h"
#include "Singleton.h"

#include <concepts>
#include <iostream>
#include <map>
#include <string>
#include <utility>

namespace nu {
	class ResourceManager :
		public Singleton<ResourceManager> {
	public:
		void RemoveAll() {
			a_resources.clear();
		}

		template<typename T, typename... Args>
			requires std::derived_from<T, Resource>
		res_t<T> Get(
			const std::string& name,
			Args&&... args
		);

		template<typename T, typename... Args>
			requires std::derived_from<T, Resource>
		res_t<T> GetWithID(
			const std::string& id,
			const std::string& name,
			Args&&... args
		);

	private:
		friend class Singleton<ResourceManager>;

		ResourceManager() = default;

	private:
		std::map<
			std::string,
			res_t<Resource>
		> a_resources;
	};

	template<typename T, typename... Args>
		requires std::derived_from<T, Resource>
	res_t<T> ResourceManager::Get(
		const std::string& name,
		Args&&... args
	) {
		return GetWithID<T>(
			name,
			name,
			std::forward<Args>(args)...
		);
	}

	template<typename T, typename... Args>
		requires std::derived_from<T, Resource>
	res_t<T> ResourceManager::GetWithID(
		const std::string& id,
		const std::string& name,
		Args&&... args
	) {
		auto iter = a_resources.find(id);

		// Return the existing resource if it
		// has already been loaded.
		if (iter != a_resources.end()) {
			res_t<Resource> baseResource =
				iter->second;

			res_t<T> resource =
				std::dynamic_pointer_cast<T>(
					baseResource
				);

			if (resource == nullptr) {
				std::cerr
					<< "Resource type mismatch: "
					<< id
					<< '\n';

				return nullptr;
			}

			return resource;
		}

		// Create and load a new resource.
		res_t<T> resource =
			std::make_shared<T>();

		if (!resource->Load(
			name,
			std::forward<Args>(args)...
		)) {
			std::cerr
				<< "Could not load resource: "
				<< name
				<< '\n';

			return nullptr;
		}

		// Store the resource for future requests.
		a_resources[id] = resource;

		return resource;
	}

	inline ResourceManager& Resources() {
		return ResourceManager::Instance();
	}
}