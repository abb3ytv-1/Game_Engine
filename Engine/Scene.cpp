#include "pch.h"
#include "Scene.h"
#include "Renderer.h"
#include "PrototypeManager.h"
#include "Serializer.h"

#include <algorithm>
#include <iostream>
#include <utility>

namespace nu {

	void Scene::AddActor(std::unique_ptr<Actor> actor) {
		if (actor != nullptr) {
			a_pendingActors.push_back(std::move(actor));
		}
	}


	std::unique_ptr<Actor> Scene::Instantiate(
		const std::string& prototype
	) {
		if (a_prototypeManager == nullptr) {
			std::cerr
				<< "Scene has no PrototypeManager."
				<< std::endl;

			return nullptr;
		}

		return a_prototypeManager->Instantiate(prototype);
	}


	Actor* Scene::FindActorByTag(
		const std::string& tag
	) {
		for (auto& actor : a_actors) {
			if (actor != nullptr &&
				actor->GetTag() == tag) {
				return actor.get();
			}
		}

		for (auto& actor : a_pendingActors) {
			if (actor != nullptr &&
				actor->GetTag() == tag) {
				return actor.get();
			}
		}

		return nullptr;
	}


	void Scene::AddPendingActors() {
		for (auto& actor : a_pendingActors) {
			a_actors.push_back(std::move(actor));
		}

		a_pendingActors.clear();
	}


	void Scene::RemoveDestroyedActors() {
		a_actors.erase(
			std::remove_if(
				a_actors.begin(),
				a_actors.end(),
				[](const std::unique_ptr<Actor>& actor) {
					return actor == nullptr ||
						actor->IsDestroyed();
				}
			),
			a_actors.end()
		);
	}


	void Scene::Update(float dt) {

		// Actors created during the previous frame
		// become active.
		AddPendingActors();

		for (auto& actor : a_actors) {
			if (actor != nullptr &&
				!actor->IsDestroyed()) {

				actor->Update(dt);
			}
		}

		RemoveDestroyedActors();
	}


	void Scene::Draw(
		const Renderer& renderer
	) const {

		for (const auto& actor : a_actors) {
			if (actor != nullptr &&
				!actor->IsDestroyed()) {

				actor->Draw(renderer);
			}
		}
	}


	void Scene::RemoveAll() {
		a_pendingActors.clear();
		a_actors.clear();
	}

}