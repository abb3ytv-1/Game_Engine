#pragma once

#include "Font.h"
#include "Game.h"
#include "Model.h"
#include "Resource.h"
#include "Scene.h"
#include "Text.h"
#include "Texture.h"
#include "Vector2.h"

#include <vector>

namespace nu {
	class Player;

	class SpaceGame : public Game {
	public:
		enum class GameState {
			Title,
			StartGame,
			StartLevel,
			Game,
			GameOver
		};

	public:
		SpaceGame();

		int Run();

		bool Initialize() override;
		void Shutdown() override;

		void Update(float dt) override;

		void Draw(
			const Renderer& renderer
		) override;

	private:
		bool LoadAudio();

		void CreateActors();

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
		Scene a_gameScene;

		Model a_playerModel;
		Model a_enemyModel;
		Model a_fastEnemyModel;
		Model a_bulletModel;

		Player* a_player{ nullptr };

		// Shared resources
		res_t<Font> a_font;
		res_t<Texture> a_texture;

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

		bool a_quit{ false };

		GameState a_gameState{
			GameState::StartGame
		};
	};
}