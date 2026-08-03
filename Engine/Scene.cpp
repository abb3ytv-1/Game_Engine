#include "pch.h"
#include "Scene.h"
#include "Renderer.h"

#include <algorithm>
#include <utility>

namespace nu {
	void Scene::AddActor(std::unique_ptr<Actor> actor) {
		if (actor != nullptr) {
			a_pendingActors.push_back( std::move(actor) );
		}
	}

	void Scene::AddPendingActors() {
		for (auto& actor : a_pendingActors) {
			a_actors.push_back( std::move(actor) );
		}

		a_pendingActors.clear();
	}

	void Scene::RemoveDestroyedActors() {
		a_actors.erase( std::remove_if( a_actors.begin(), a_actors.end(), [](const std::unique_ptr<Actor>& actor) {
					return ( actor == nullptr || actor->IsDestroyed() );
				}
			), a_actors.end()
		);
	}

	void Scene::Update(float dt) {
		// Actors created during the previous frame become active.
		AddPendingActors();

		// Update active actors.
		for (auto& actor : a_actors) {
			if ( actor != nullptr && !actor->IsDestroyed() ) {
				actor->Update(dt);
			}
		}

		// Delete actors that were destroyed during Update.
		RemoveDestroyedActors();
	}

	void Scene::Draw(const Renderer& renderer) const {
		for (const auto& actor : a_actors) {
			if (
				actor != nullptr &&
				!actor->IsDestroyed()
				) {
				actor->Draw(renderer);
			}
		}
	}

	void Scene::RemoveAll() {
		a_pendingActors.clear();
		a_actors.clear();
	}
}