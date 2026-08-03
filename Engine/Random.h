#pragma once

#include <stdlib.h>

namespace nu {
	inline int RandomInt() {
		return rand();
	}

	/// <summary>
	/// Get randon integer between [0, max)
	/// </summary>
	/// <param name="max">exclusive max</param>
	/// <returns>Random between 0 and max</returns>
	inline int RandomInt(int max) {
		return rand() % max;
	}

	/// <summary>
	/// Get random integer between [min, max)
	/// </summary>
	/// <param name="min">inclusive min</param>
	/// <param name="max">exclusive max</param>
	/// <returns>Random between min and max</returns>
	inline int RandomInt(int min, int max) {
		return min + RandomInt(max - min);
	}

	inline float RandomFloat() {
		// rand() = 0 <- RAND_MAX
		return rand() / (float)RAND_MAX;
	}

	inline float RandomFloat(float max) {
		// (0 <-> 1) * max
		return RandomFloat() * max;
	}
	
	inline float RandomFloat(float min, float max) {
		// (min + 0 <-> 1) * (max - min)
		return min + RandomFloat() * (max - min);
	}
}
