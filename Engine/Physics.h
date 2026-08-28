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

		const b2BodyId* GetDemoBodies() const {
			return a_demoBodies;
		}

		int GetDemoBodyCount() const {
			return 3;
		}

	private:
		b2WorldId a_worldId;
		b2BodyId a_demoBodies[3]{};
	};
}
