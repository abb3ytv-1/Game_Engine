#pragma once

#include <fmod.hpp>

#include <map>
#include <string>

namespace nu {
	class Audio {
	public:
		Audio() = default;

		bool Initialize();
		void Shutdown();
		void Update();

		bool AddSound(
			const std::string& name,
			const std::string& filename
		);

		bool PlaySound(
			const std::string& name
		);

	private:
		bool CheckFMODResult(
			FMOD_RESULT result
		);

	private:
		FMOD::System* a_fmodSystem{ nullptr };

		std::map<
			std::string,
			FMOD::Sound*
		> a_sounds;
	};
}