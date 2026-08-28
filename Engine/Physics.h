#pragma once

#include <box2d/box2d.h>
#include <memory>

namespace nu
{
	class Physics
	{
	public:
		Physics() = default;

		bool Initialize();
		void Shutdown();

		void Update(float dt);

	private:
		b2WorldId a_worldId;
	};
}
