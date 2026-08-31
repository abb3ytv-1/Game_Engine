#pragma once
#include "Vector2.h"

namespace nu {
	struct Transform {
		Vector2 position{ 0.0f, 0.0f };
		float rotation = 0.0f;
		float scale = 1.0f;
	};
}