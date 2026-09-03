#pragma once

#include "../Engine/Renderer/Font.h"
#include "../Engine/Framework/Game.h"
#include "../Engine/Renderer/Model.h"
#include "../Engine/Resources/Resource.h"
#include "../Engine/Framework/Scene.h"
#include "../Engine/Renderer/Text.h"
#include "../Engine/Renderer/Texture.h"
#include "../Engine/TextureFrames.h"
#include "../Engine/Math/Vector2.h"
#include "../Engine/Serialization/PrototypeManager.h"

#include <vector>
#include <memory>

namespace nu {

	class FishGame : public Game {
	public:
		enum class GameState {
			Title,
			StartGame,
			StartLevel,
			Game,
			GameOver
		};

	public:
		FishGame();

		int Run();

		bool Initialize() override;
		void Shutdown() override;

		void Update(float dt) override;

		void Draw(
			const Renderer& renderer
		) override;

	private:
		bool LoadAudio();

		// Scene / prototype creation
		void CreateActors();

		void DrawPhysicsDemo(const Renderer& renderer);

		void AddEnemy(
			const Vector2& position,
			float speed
		);

		void AddFastEnemy(
			const Vector2& position,
			float speed
		);

		void ProcessEvents();

		void HandleAudioInput();
		void HandlePlayerInput(float dt);
		void HandleShooting();
		void HandleMouseInput();

		void CheckCollisions();

		void CreateExplosion(
			const Vector2& position,
			const Color& color,
			int particleCount = 100
		);

		void EmitPlayerParticle();

		void StartNewGame();
		void EndGame();

		void UpdateHUDText();

		void StartNextLevel();
		void SpawnLevelEnemies();

		bool HasActiveEnemies() const;

		void LoadHighScore();
		void SaveHighScore();

	private:
		PrototypeManager a_prototypeManager;

		Model a_playerModel;
		Model a_enemyModel;
		Model a_fastEnemyModel;
		Model a_bulletModel;

		Actor* a_player{ nullptr };

		// Shared resources
		res_t<Font> a_font;

		res_t<Texture> a_texture;
		res_t<Texture> a_enemyTexture;
		res_t<Texture> a_fastEnemyTexture;
		res_t<Texture> a_bulletTexture;
		res_t<Texture> a_backgroundTexture;
		res_t<Texture> a_particleTexture;
		res_t<Texture> a_animationTexture;
		res_t<TextureFrames> a_animationFrames;

		Text a_stateText;
		Text a_hudText;

		std::vector<Vector2> a_mousePoints;
		std::vector<bool> a_startsNewShape;

		int a_score{ 0 };
		int a_highScore{ 0 };
		int a_lives{ 3 };
		int a_level{ 1 };

		float a_rotationSpeed{ 180.0f };
		float a_playerInvincibilityTimer{ 0.0f };
		float a_levelStartTimer{ 0.0f };

		float a_playerSpriteRotationOffsetDeg{ -90.0f };

		bool a_quit{ false };

		GameState a_gameState{
			GameState::StartGame
		};
	};

}