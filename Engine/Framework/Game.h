#pragma once

namespace nu {
	class Scene;
	class Renderer;

	class Game {
	public:
		Game() = default;

		Game(Scene* scene) :
			a_scene{ scene }
		{}

		virtual ~Game() = default;

		virtual bool Initialize() {
			return true;
		}

		virtual void Shutdown() {}

		virtual void Update(float dt);

		virtual void Draw(
			const Renderer& renderer
		);

		void SetScene(Scene* scene) {
			a_scene = scene;
		}

	protected:
		Scene* a_scene{ nullptr };
	};
}