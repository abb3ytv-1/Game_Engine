#pragma once

#include <box2d/box2d.h>

namespace nu {
	class Physics {
	public:
		Physics() = default;

		bool Initialize();
		void Shutdown();
		void Update(float dt);

		b2WorldId GetWorldId() const { return a_worldId; }

	private:
		b2WorldId a_worldId;
	};
}
