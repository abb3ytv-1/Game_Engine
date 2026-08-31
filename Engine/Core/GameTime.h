#pragma once
#include <cstdint>

namespace nu
{
	class Time
	{
	public:
		Time();

		void Reset();
		void Tick();

		float GetTime() const { return TicksToSeconds(a_currentTicks - a_startTicks); }
		float GetDeltaTime() const { return TicksToSeconds(a_deltaTicks) * a_timeScale; }

		float TicksToSeconds(uint64_t ticks) const { return (float)ticks / 1'000'000'000; }

		void SetTimeScale(float timeScale) { a_timeScale = timeScale; }
		float GetTimeScale() const { return a_timeScale; }

	private:
		float a_timeScale = 1.0f;

		uint64_t a_startTicks = 0;
		uint64_t a_currentTicks = 0;
		uint64_t a_frameTicks = 0;
		uint64_t a_deltaTicks = 0;
	};
}