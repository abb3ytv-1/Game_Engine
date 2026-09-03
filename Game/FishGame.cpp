#include "FishGame.h"

#include "../Engine/Framework/PlayerComponent.h"
#include "../Engine/Framework/EnemyAIComponent.h"
#include "../Engine/Framework/BulletComponent.h"
#include "../Engine/Framework/RangedEnemyAIComponent.h"

#include "../Engine/SpriteAnimationRendererComponent.h"
#include "../Engine/TextureFrames.h"
#include "../Engine/Renderer/SpriteRendererComponent.h"
#include "../Engine/Renderer/Model.h"
#include "../Engine/Renderer/ParticleSystem.h"
#include "../Engine/TilemapRendererComponent.h"

#include "../Engine/Physics/CircleCollider2DComponent.h"
#include "../Engine/Physics/RigidBodyComponent.h"

#include "../Engine/Core/File.h"
#include "../Engine/Math/MathUtils.h"
#include "../Engine/Core/Random.h"
#include "../Engine/Core/Factory.h"

#include "../Engine/Engine.h"

#include "Assets.h"

#include <SDL3/SDL.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <filesystem>
#include <algorithm>

using namespace nu;

FishGame::FishGame() = default;

int FishGame::Run() {
	if (!Initialize()) {
		return 1;
	}

	Renderer& renderer = engine.GetRenderer();

	while (!a_quit) {
		ProcessEvents();

		if (a_quit) {
			break;
		}

		engine.Update();

		float dt =
			engine.GetTime().GetDeltaTime();

		Update(dt);
		Draw(renderer);
	}

	Shutdown();

	return 0;
}

bool FishGame::Initialize() {
	if (!Game::Initialize()) {
		return false;
	}

	a_scene = std::make_unique<Scene>();

	if (!SetWorkingDirectory("FishGame")) {
		std::cerr
			<< "Could not set FishGame working directory.\n";

		return false;
	}

	LoadHighScore();

	a_font = Resources().GetWithID<Font>(
		"game_font",
		"Fonts/New Moon.ttf",
		48.0f
	);

	if (a_font == nullptr) {
		return false;
	}

	res_t<Font> reusedFont =
		Resources().GetWithID<Font>(
			"game_font",
			"Fonts/New Moon.ttf",
			48.0f
		);

	if (reusedFont != a_font) {
		std::cerr
			<< "Font resource was not reused.\n";

		return false;
	}

	a_stateText.SetFont(a_font);
	a_hudText.SetFont(a_font);

	std::string titleMessage =
		"Space Adventure | High Score: " +
		std::to_string(a_highScore) +
		" | Press Enter to Start";

	if (!a_stateText.Create(
		engine.GetRenderer(),
		titleMessage,
		Color{ 0.45f, 0.85f, 1.0f }
	)) {
		return false;
	}

	std::string hudMessage =
		"Score: 0 | High Score: " +
		std::to_string(a_highScore) +
		" | Lives: 3 | Level: 1";

	if (!a_hudText.Create(
		engine.GetRenderer(),
		hudMessage,
		Color{ 1.0f, 1.0f, 1.0f }
	)) {
		return false;
	}

	a_texture = Resources().Get<Texture>(
		"Textures/large_grey_01.png",
		engine.GetRenderer()
	);

	if (a_texture == nullptr) {
		a_texture = Resources().Get<Texture>(
			"textures/large_grey_01.png",
			engine.GetRenderer()
		);
	}

	if (a_texture == nullptr) {
		std::cerr
			<< "Could not load texture resource.\n";

		return false;
	}

	res_t<Texture> reusedTexture =
		Resources().Get<Texture>(
			"Textures/large_grey_01.png",
			engine.GetRenderer()
		);

	if (reusedTexture != a_texture) {
		std::cerr
			<< "Texture resource was not reused.\n";

		return false;
	}

	if (!LoadAudio()) {
		return false;
	}

	a_playerModel = CreatePlayerModel();
	a_enemyModel = CreateEnemyModel();
	a_fastEnemyModel = CreateFastEnemyModel();
	a_bulletModel = CreateBulletModel();

	a_enemyTexture = Resources().Get<Texture>(
		"Textures/large_grey_02.png",
		engine.GetRenderer()
	);

	a_fastEnemyTexture = Resources().Get<Texture>(
		"Textures/large_purple_01.png",
		engine.GetRenderer()
	);

	a_rangedEnemyTexture = Resources().Get<Texture>(
		"Textures/eye.png",
		engine.GetRenderer()
	);

	if (a_rangedEnemyTexture != nullptr) {
		a_rangedEnemyFrames =
			Resources().GetWithID<TextureFrames>(
				"eye_frames",
				"Textures/eye.png",
				1200,
				600,
				150,
				600
			);
	}

	a_bulletTexture = Resources().Get<Texture>(
		"Textures/missile-2.png",
		engine.GetRenderer()
	);

	a_foodTexture = Resources().Get<Texture>(
		"Textures/coin.png",
		engine.GetRenderer()
	);

	a_foodTexture = Resources().Get<Texture>(
		"Textures/coin.png",
		engine.GetRenderer()
	);

	if (a_foodTexture != nullptr) {
		a_foodAnimationFrames =
			Resources().GetWithID<TextureFrames>("coin_frames", "Textures/coin.png", 256, 32, 32, 32);
	}

	a_backgroundTexture = Resources().Get<Texture>(
		"Textures/background.png",
		engine.GetRenderer()
	);

	a_particleTexture = Resources().Get<Texture>(
		"Textures/spark_06.png",
		engine.GetRenderer()
	);

	if (a_particleTexture != nullptr) {
		engine.GetPS().SetTexture(a_particleTexture);
	}

	a_animationTexture = Resources().Get<Texture>(
		"Textures/ghost-idle.png",
		engine.GetRenderer()
	);

	if (a_animationTexture == nullptr) {
		std::cerr << "Could not load ghost animation texture.\n";
		return false;
	}

	a_animationFrames =
		Resources().GetWithID<TextureFrames>(
			"ghost_idle_frames",
			"Textures/ghost-idle.png",
			448,
			80,
			64,
			80
		);

	if (a_animationFrames == nullptr) {
		std::cerr << "Could not load ghost animation frames.\n";
		return false;
	}

	a_gameState = GameState::StartGame;

	return true;
}

bool FishGame::LoadAudio() {
	Audio& audio = engine.GetAudio();

	bool loaded = true;

	loaded &= audio.AddSound(
		"bass",
		"Audio/bass.wav"
	);

	loaded &= audio.AddSound(
		"snare",
		"Audio/snare.wav"
	);

	loaded &= audio.AddSound(
		"clap",
		"Audio/clap.wav"
	);

	loaded &= audio.AddSound(
		"open-hat",
		"Audio/open-hat.wav"
	);

	loaded &= audio.AddSound(
		"cowbell",
		"Audio/cowbell.wav"
	);

	return loaded;
}

void FishGame::CreateActors() {
	// Tilemap - scaled to cover the full screen
	auto tilemapActor = std::make_unique<nu::Actor>();

	Renderer& renderer = engine.GetRenderer();

	res_t<Tilemap> tilemap =
		Resources().Get<Tilemap>(
			"Maps/world.json",
			renderer
		);

	float mapScale = 1.0f;

	if (tilemap != nullptr) {
		float mapPixelWidth =
			static_cast<float>(tilemap->GetWidth()) *
			static_cast<float>(tilemap->GetTileWidth());

		float mapPixelHeight =
			static_cast<float>(tilemap->GetHeight()) *
			static_cast<float>(tilemap->GetTileHeight());

		if (mapPixelWidth > 0.0f && mapPixelHeight > 0.0f) {
			float screenW = static_cast<float>(renderer.GetWidth());
			float screenH = static_cast<float>(renderer.GetHeight());

			mapScale = std::max(
				screenW / mapPixelWidth,
				screenH / mapPixelHeight
			);
		}
	}
	else {
		std::cerr << "Could not load tilemap resource for scaling.\n";
	}

	tilemapActor->SetTransform(
		Transform{ Vector2{ 0.0f, 0.0f }, 0.0f, mapScale }
	);

	auto tilemapRenderer = std::make_unique<nu::TilemapRendererComponent>();
	tilemapRenderer->SetTilemapName("Maps/world.json");
	tilemapActor->AddComponent(std::move(tilemapRenderer));

	a_scene->AddActor(std::move(tilemapActor));

	// Player - RESTORED (this block had gone missing)
	std::unique_ptr<nu::Actor> playerActor = std::make_unique<nu::Actor>();

	playerActor->SetTransform(
		Transform{
			Vector2{ renderer.GetWidth() * 0.5f, renderer.GetHeight() * 0.5f },
			0.0f,
			10.0f
		}
	);

	playerActor->a_model = a_playerModel;

	playerActor->AddComponent(std::make_unique<nu::PlayerComponent>(300.0f, 0));
	playerActor->AddComponent(std::make_unique<nu::RigidBodyComponent>());

	a_player = playerActor.get();

	if (a_player != nullptr) {
		a_player->SetCollisionRadius(8.0f);

		if (a_animationTexture != nullptr && a_animationFrames != nullptr) {
			a_player->AddComponent(
				std::make_unique<nu::SpriteAnimationRendererComponent>(
					a_animationTexture,
					a_animationFrames,
					2.0f,
					Vector2{ 0.5f, 0.5f },
					8.0f
				)
			);
		}
	}

	a_scene->AddActor(std::move(playerActor));

	// Food pickups - collect a_foodRequired of these to win
	for (int i = 0; i < a_foodRequired; i++) {
		Vector2 position{
			RandomFloat(100.0f, static_cast<float>(renderer.GetWidth() - 100)),
			RandomFloat(100.0f, static_cast<float>(renderer.GetHeight() - 100))
		};

		AddFood(position);
	}

	// Enemies
	AddEnemy(Vector2{ 200.0f, 200.0f }, 100.0f);
	AddEnemy(Vector2{ 1700.0f, 300.0f }, 125.0f);
	AddEnemy(Vector2{ 400.0f, 900.0f }, 75.0f);
	AddRangedEnemy(Vector2{ 900.0f, 700.0f }, 90.0f);

}

void FishGame::AddFood(const Vector2& position)
{
	auto food = std::make_unique<nu::Actor>();

	food->SetTransform(Transform{ position, 0.0f, 5.0f });

	food->a_model = a_bulletModel;
	food->SetCollisionRadius(5.0f);

	if (a_foodTexture != nullptr && a_foodAnimationFrames != nullptr) {
		const float visualScale = 0.8f;

		food->AddComponent(
			std::make_unique<nu::SpriteAnimationRendererComponent>(
				a_foodTexture,
				a_foodAnimationFrames,
				visualScale,
				Vector2{ 0.5f, 0.5f },
				6.0f
			)
		);
	}
	else if (a_bulletTexture != nullptr) {
		Vector2 texSize = a_bulletTexture->GetSize();
		if (texSize.x > 0.0f) {
			float scale = std::clamp((food->GetCollisionRadius() * 2.0f) / texSize.x, 0.01f, 5.0f);
			food->AddComponent(
				std::make_unique<nu::SpriteRendererComponent>(a_bulletTexture, scale, Vector2{ 0.5f, 0.5f })
			);
		}
	}

	a_foodActors.push_back(food.get());
	a_scene->AddActor(std::move(food));
}

void FishGame::AddEnemy(const Vector2& position, float speed) {
	if (a_player == nullptr) {
		return;
	}

	auto enemy = std::make_unique<nu::Actor>();
	enemy->SetTransform(Transform{ position, 0.0f, 8.0f });
	enemy->a_model = a_enemyModel;
	enemy->AddComponent(std::make_unique<nu::EnemyAIComponent>(a_player, speed));
	enemy->AddComponent(std::make_unique<nu::RigidBodyComponent>());
	enemy->SetCollisionRadius(8.0f);

	if (enemy && a_enemyTexture != nullptr) {
		Vector2 texSize = a_enemyTexture->GetSize();
		if (texSize.x > 0.0f) {
			float desiredDiameter = enemy->GetCollisionRadius() * 2.0f;
			float scale = desiredDiameter / texSize.x;
			scale = std::clamp(scale, 0.01f, 5.0f);
			enemy->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_enemyTexture, scale, Vector2{0.5f,0.0f}));
		} else {
			enemy->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_enemyTexture, 1.0f, Vector2{0.5f,0.0f}));
		}
	}


	a_scene->AddActor(std::move(enemy));
}

void FishGame::AddFastEnemy(const Vector2& position,float speed) {
	if (a_player == nullptr) {
		return;
	}

	auto enemy = std::make_unique<nu::Actor>();
	enemy->SetTransform(Transform{ position, 0.0f, 6.0f });
	enemy->a_model = a_fastEnemyModel;
	enemy->AddComponent(std::make_unique<nu::EnemyAIComponent>(a_player, speed));
	enemy->AddComponent(std::make_unique<nu::RigidBodyComponent>());
	enemy->SetCollisionRadius(6.0f);

	if (enemy && a_fastEnemyTexture != nullptr) {
		Vector2 texSize = a_fastEnemyTexture->GetSize();
		if (texSize.x > 0.0f) {
			float desiredDiameter = enemy->GetCollisionRadius() * 2.0f;
			float scale = desiredDiameter / texSize.x;
			scale = std::clamp(scale, 0.01f, 5.0f);
			enemy->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_fastEnemyTexture, scale, Vector2{0.5f,0.0f}));
		} else {
			enemy->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_fastEnemyTexture, 1.0f, Vector2{0.5f,0.0f}));
		}
	}
	else if (enemy && a_enemyTexture != nullptr) {
		Vector2 texSize = a_enemyTexture->GetSize();
		if (texSize.x > 0.0f) {
			float desiredDiameter = enemy->GetCollisionRadius() * 2.0f;
			float scale = desiredDiameter / texSize.x;
			scale = std::clamp(scale, 0.01f, 5.0f);
			enemy->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_enemyTexture, scale, Vector2{0.5f,0.0f}));
		} else {
			enemy->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_enemyTexture, 1.0f, Vector2{0.5f,0.0f}));
		}
	}

	a_scene->AddActor(std::move(enemy));
}

void FishGame::AddRangedEnemy(const Vector2& position, float speed) {
	if (a_player == nullptr) {
		return;
	}

	auto enemy = std::make_unique<nu::Actor>();
	enemy->SetTransform(Transform{ position, 0.0f, 7.0f });
	enemy->a_model = a_enemyModel;
	enemy->AddComponent(
		std::make_unique<nu::RangedEnemyAIComponent>(a_player, speed, 350.0f, 1.5f)
	);
	enemy->AddComponent(std::make_unique<nu::RigidBodyComponent>());
	enemy->SetCollisionRadius(7.0f);

	if (a_rangedEnemyTexture != nullptr && a_rangedEnemyFrames != nullptr) {
		enemy->AddComponent(
			std::make_unique<nu::SpriteAnimationRendererComponent>(
				a_rangedEnemyTexture,
				a_rangedEnemyFrames,
				0.8f,
				Vector2{ 0.5f, 0.5f },
				6.0f
			)
		);
	}
	else if (a_enemyTexture != nullptr) {
		// fallback if eye.png/frames failed to load
		Vector2 texSize = a_enemyTexture->GetSize();
		if (texSize.x > 0.0f) {
			float scale = std::clamp((enemy->GetCollisionRadius() * 2.0f) / texSize.x, 0.01f, 5.0f);
			enemy->AddComponent(
				std::make_unique<nu::SpriteRendererComponent>(a_enemyTexture, scale, Vector2{ 0.5f, 0.0f })
			);
		}
	}

	a_scene->AddActor(std::move(enemy));
}

void FishGame::ProcessEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			a_quit = true;
			continue;
		}

		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.scancode == SDL_SCANCODE_ESCAPE) {
				a_quit = true;
				continue;
			}

			bool enterPressed = event.key.scancode == SDL_SCANCODE_RETURN || event.key.scancode == SDL_SCANCODE_KP_ENTER;

			bool canStartGame = a_gameState == GameState::Title || a_gameState == GameState::StartGame || a_gameState == GameState::GameOver || a_gameState == GameState::Win;

			if (enterPressed && canStartGame) {
				StartNewGame();
			}
		}
	}
}

void FishGame::Update(float dt) {
	switch (a_gameState) {
	case GameState::Title:
		break;

	case GameState::StartGame:
		break;

	case GameState::StartLevel:
		a_levelStartTimer -= dt;

		if (a_levelStartTimer <= 0.0f) {
			SpawnLevelEnemies();
			a_gameState = GameState::Game;
		}
		break;

	case GameState::Game:
		if (a_playerInvincibilityTimer > 0.0f) {
			a_playerInvincibilityTimer -= dt;
		}

		HandleAudioInput();
		HandlePlayerInput(dt);
		HandleEnemyShooting(dt);
		EmitPlayerParticle();
		HandleMouseInput();

		Game::Update(dt);

		CheckCollisions();
		break;

	case GameState::GameOver:
		break;

	case GameState::Win:
		break;

	default:
		break;
	}
}

void FishGame::HandleAudioInput() {
	Input& input = engine.GetInput();
	Audio& audio = engine.GetAudio();

	if (input.GetKeyPress(SDL_SCANCODE_1)) { audio.PlaySound("bass"); }
	if (input.GetKeyPress(SDL_SCANCODE_2)) { audio.PlaySound("snare"); }
	if (input.GetKeyPress(SDL_SCANCODE_3)) { audio.PlaySound("clap"); }
	if (input.GetKeyPress(SDL_SCANCODE_4)) { audio.PlaySound("open-hat"); }
	if (input.GetKeyPress(SDL_SCANCODE_5)) { audio.PlaySound("cowbell"); }
}

void FishGame::HandlePlayerInput(float dt) {
	if (a_player == nullptr || a_player->IsDestroyed()) { return; }

	Input& input = engine.GetInput();

	float rotation = a_player->GetTransform().rotation;

	if (input.GetKeyDown(SDL_SCANCODE_LEFT)) { rotation -= a_rotationSpeed * dt; }
	if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) { rotation += a_rotationSpeed * dt; }

	a_player->SetRotation(rotation);

	Vector2 direction{ 0.0f, 0.0f };

	if (input.GetKeyDown(SDL_SCANCODE_W)) { direction.y -= 1.0f; }
	if (input.GetKeyDown(SDL_SCANCODE_S)) { direction.y += 1.0f; }
	if (input.GetKeyDown(SDL_SCANCODE_A)) { direction.x -= 1.0f; }
	if (input.GetKeyDown(SDL_SCANCODE_D)) { direction.x += 1.0f; }
	if (direction.LengthSqr() > 0.0f) { direction = direction.Normalized(); }
	if (a_player) { auto* pc = a_player->GetComponent<nu::PlayerComponent>(); float speed = pc ? pc->GetSpeed() : 0.0f; a_player->SetVelocity(direction * speed); }
	if (input.GetKeyPress(SDL_SCANCODE_SPACE)) { HandleShooting(); }

}

void FishGame::HandleShooting() {
	if (a_player == nullptr || a_player->IsDestroyed() ) { return; }

	float rotation = a_player->GetTransform().rotation + a_playerSpriteRotationOffsetDeg;

	Vector2 forward{ 1.0f, 0.0f };

	forward = forward.Rotate(rotation * DegToRad);

	float spawnDistance = a_player->GetCollisionRadius() + 10.0f;

	Vector2 bulletPosition = a_player->GetTransform().position + (forward * spawnDistance);

	std::unique_ptr<nu::Actor> bullet = std::make_unique<nu::Actor>();
	bullet->SetTransform(Transform{ bulletPosition, rotation, 4.0f });
	bullet->a_model = a_bulletModel;

	Vector2 forwardLocal{ 1.0f, 0.0f };
	forwardLocal = forwardLocal.Rotate(rotation * DegToRad);
	bullet->AddComponent(std::make_unique<nu::RigidBodyComponent>());
	bullet->SetVelocity(forwardLocal * 700.0f);
	bullet->SetDamping(0.0f);
	bullet->SetLifespan(2.0f);
	bullet->SetCollisionRadius(2.0f);
	bullet->AddComponent(std::make_unique<nu::BulletComponent>());

	if (bullet && a_bulletTexture != nullptr) {
		Vector2 texSize = a_bulletTexture->GetSize();
		if (texSize.x > 0.0f) {
			float desiredDiameter = bullet->GetCollisionRadius() * 2.0f;
			float scale = desiredDiameter / texSize.x;
			scale = std::clamp(scale, 0.005f, 5.0f);
			bullet->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_bulletTexture, scale, Vector2{0.5f,0.5f}));
		} else {
			bullet->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_bulletTexture, 1.0f, Vector2{0.5f,0.5f}));
		}
	}

	a_scene->AddActor(std::move(bullet));

	engine.GetAudio().PlaySound("snare");
}

void FishGame::HandleEnemyShooting(float dt) {
	if (a_player == nullptr || a_player->IsDestroyed()) {
		return;
	}

	auto& actors = a_scene->GetActors();

	for (auto& actor : actors) {
		if (actor->IsDestroyed()) {
			continue;
		}

		auto* ranged = actor->GetComponent<nu::RangedEnemyAIComponent>();
		if (ranged == nullptr) {
			continue;
		}

		if (!ranged->ConsumeShootRequest()) {
			continue;
		}

		Vector2 toPlayer =
			a_player->GetTransform().position - actor->GetTransform().position;

		if (toPlayer.LengthSqr() <= 0.0f) {
			continue;
		}

		Vector2 direction = toPlayer.Normalized();

		auto bullet = std::make_unique<nu::Actor>();
		bullet->SetTransform(Transform{ actor->GetTransform().position, 0.0f, 4.0f });
		bullet->a_model = a_bulletModel;

		bullet->AddComponent(std::make_unique<nu::RigidBodyComponent>());
		bullet->SetVelocity(direction * 400.0f);
		bullet->SetDamping(0.0f);
		bullet->SetLifespan(2.5f);
		bullet->SetCollisionRadius(2.0f);
		bullet->AddComponent(std::make_unique<nu::BulletComponent>(true));   // isEnemyOwned = true

		if (a_bulletTexture != nullptr) {
			Vector2 texSize = a_bulletTexture->GetSize();
			if (texSize.x > 0.0f) {
				float scale = std::clamp(
					(bullet->GetCollisionRadius() * 2.0f) / texSize.x, 0.005f, 5.0f
				);
				bullet->AddComponent(
					std::make_unique<nu::SpriteRendererComponent>(a_bulletTexture, scale, Vector2{ 0.5f, 0.5f })
				);
			}
		}

		a_scene->AddActor(std::move(bullet));
		engine.GetAudio().PlaySound("open-hat");
	}
}

void FishGame::HandleMouseInput() {
	Input& input = engine.GetInput();

}

void FishGame::CheckCollisions() {
	auto& actors = a_scene->GetActors();

	auto isAnyEnemy = [](Actor& a) {
		return a.GetComponent<nu::EnemyAIComponent>() != nullptr ||
			a.GetComponent<nu::RangedEnemyAIComponent>() != nullptr;
		};

	// Player bullets vs any enemy
	for (auto& actor : actors) {
		auto* bulletTag = actor->GetComponent<nu::BulletComponent>();
		if (bulletTag == nullptr || actor->IsDestroyed() || bulletTag->IsEnemyOwned()) {
			continue;
		}

		for (auto& otherActor : actors) {
			if (otherActor->IsDestroyed() || !isAnyEnemy(*otherActor)) {
				continue;
			}

			if (actor->IsColliding(*otherActor)) {
				Vector2 explosionPosition = otherActor->GetTransform().position;

				actor->Destroy();
				otherActor->Destroy();

				engine.GetAudio().PlaySound("clap");
				CreateExplosion(explosionPosition, Color{ 1.0f, 0.65f, 0.2f }, 100);

				a_score++;
				UpdateHUDText();
				break;
			}
		}
	}

	if (a_player == nullptr || a_player->IsDestroyed()) {
		return;
	}

	// Food pickups
	if (a_playerInvincibilityTimer <= 0.0f) {
		for (auto* food : a_foodActors) {
			if (food == nullptr || food->IsDestroyed()) {
				continue;
			}

			if (!a_player->IsColliding(*food)) {
				continue;
			}

			food->Destroy();
			a_foodCollected++;
			a_score++;

			engine.GetAudio().PlaySound("open-hat");
			CreateExplosion(food->GetTransform().position, Color{ 1.0f, 0.9f, 0.2f }, 40);
			UpdateHUDText();

			if (a_foodCollected >= a_foodRequired) {
				WinGame();
				return;
			}

			break;
		}
	}

	if (a_playerInvincibilityTimer > 0.0f) {
		return;
	}

	bool gameOver = false;
	bool tookHit = false;
	Vector2 hitPosition{};

	// Melee contact with either enemy type
	for (auto& actor : actors) {
		if (actor->IsDestroyed() || !isAnyEnemy(*actor)) {
			continue;
		}

		if (!a_player->IsColliding(*actor)) {
			continue;
		}

		hitPosition = actor->GetTransform().position;
		actor->Destroy();
		tookHit = true;
		break;
	}

	// Enemy bullets vs player
	if (!tookHit) {
		for (auto& actor : actors) {
			auto* bulletTag = actor->GetComponent<nu::BulletComponent>();
			if (bulletTag == nullptr || actor->IsDestroyed() || !bulletTag->IsEnemyOwned()) {
				continue;
			}

			if (!a_player->IsColliding(*actor)) {
				continue;
			}

			hitPosition = actor->GetTransform().position;
			actor->Destroy();
			tookHit = true;
			break;
		}
	}

	if (tookHit) {
		CreateExplosion(hitPosition, Color{ 1.0f, 0.2f, 0.2f }, 75);
		a_lives--;

		engine.GetAudio().PlaySound("bass");
		UpdateHUDText();

		if (a_lives <= 0) {
			gameOver = true;
		}
		else {
			Renderer& renderer = engine.GetRenderer();
			a_player->SetPosition(Vector2{ renderer.GetWidth() * 0.5f, renderer.GetHeight() * 0.5f });
			a_player->SetVelocity(Vector2{ 0.0f, 0.0f });
			a_player->SetRotation(0.0f);
			a_playerInvincibilityTimer = 1.5f;
		}
	}

	if (gameOver) {
		EndGame();
		return;
	}

	if (!HasActiveEnemies()) {
		StartNextLevel();
	}
}

void FishGame::CreateExplosion(const Vector2& position, const Color& color, int particleCount) {
	ParticleSystem& particleSystem = engine.GetPS();

	for (int i = 0; i < particleCount; i++) {
		Particle particle;

		particle.position = position;
		particle.color = color;

		particle.lifespan =
			RandomFloat(0.5f, 2.0f);
		particle.velocity = Vector2{
			RandomFloat(-600.0f, 600.0f),
			RandomFloat(-600.0f, 600.0f)
		};

		particle.size = RandomFloat(4.0f, 12.0f);
		particle.rotation = RandomFloat(0.0f, 360.0f);
		particle.angularVelocity = RandomFloat(-180.0f, 180.0f);

		particleSystem.AddParticle(particle);
	}
}

void FishGame::EmitPlayerParticle() {
	if (a_player == nullptr || a_player->IsDestroyed()) { return; }

	if (a_player->GetVelocity().LengthSqr() <= 0.0f) { return; }

	float rotation = a_player->GetTransform().rotation + a_playerSpriteRotationOffsetDeg;

	Vector2 forward{ 1.0f, 0.0f };

	forward = forward.Rotate(rotation * DegToRad);

	float trailDistance = a_player->GetCollisionRadius() + 20.0f;

	Particle particle;

	particle.position = a_player->GetTransform().position - (forward * trailDistance);
	particle.position.x += RandomFloat(-5.0f, 5.0f);
	particle.position.y += RandomFloat(-5.0f, 5.0f);
	particle.color = Color{ 0.4f, 0.8f, 1.0f };
	particle.lifespan = RandomFloat(0.25f, 0.75f);
	particle.velocity = (forward * RandomFloat(-100.0f, -40.0f)) + Vector2{ RandomFloat(-30.0f, 30.0f), RandomFloat(-30.0f, 30.0f) };
	particle.size = RandomFloat(2.0f, 6.0f);
	particle.rotation = RandomFloat(0.0f, 360.0f);
	particle.angularVelocity = RandomFloat(-120.0f, 120.0f);

	engine.GetPS().AddParticle(particle);
}

void FishGame::StartNewGame() {
	a_scene->RemoveAll();

	a_player = nullptr;
	a_score = 0;
	a_lives = 3;
	a_level = 1;
	a_levelStartTimer = 0.0f;
	a_playerInvincibilityTimer = 0.0f;
	a_foodCollected = 0;
	a_foodActors.clear();

	a_mousePoints.clear();
	a_startsNewShape.clear();

	CreateActors();
	UpdateHUDText();

	a_gameState = GameState::Game;
}

void FishGame::WinGame() {
	engine.GetAudio().PlaySound("cowbell");

	a_scene->RemoveAll();
	a_player = nullptr;

	if (a_score > a_highScore) {
		a_highScore = a_score;
		SaveHighScore();
	}

	std::string message =
		"You Win! Score: " + std::to_string(a_score) +
		" | High Score: " + std::to_string(a_highScore) +
		" | Press Enter to Play Again";

	a_stateText.Create(engine.GetRenderer(), message, Color{ 0.3f, 1.0f, 0.4f });

	a_gameState = GameState::Win;
}

void FishGame::EndGame() {
	engine.GetAudio().PlaySound("cowbell");

	a_scene->RemoveAll();
	a_player = nullptr;

	if (a_score > a_highScore) {
		a_highScore = a_score;
		SaveHighScore();
	}

	std::string message = "Game Over | Score: " + std::to_string(a_score) + " | High Score: " + std::to_string(a_highScore) + " | Press Enter to Play Again";
	a_stateText.Create(engine.GetRenderer(), message, Color{ 1.0f, 0.25f, 0.25f });
	a_gameState = GameState::GameOver;
}

bool FishGame::HasActiveEnemies() const {
	for (const auto& actor : a_scene->GetActors()) {
		bool isEnemy =
			actor->GetComponent<nu::EnemyAIComponent>() != nullptr ||
			actor->GetComponent<nu::RangedEnemyAIComponent>() != nullptr;

		if (isEnemy && !actor->IsDestroyed()) {
			return true;
		}
	}
	return false;
}

void FishGame::StartNextLevel() {
	a_level++;
	a_levelStartTimer = 2.0f;

	std::string message = "Level " + std::to_string(a_level);
	a_stateText.Create(engine.GetRenderer(), message, Color{ 0.45f, 0.85f, 1.0f } );
	engine.GetAudio().PlaySound("open-hat");

	UpdateHUDText();

	a_gameState = GameState::StartLevel;
}

void FishGame::SpawnLevelEnemies() {
	Renderer& renderer = engine.GetRenderer();

	int enemyCount = a_level + 2;
	float normalEnemySpeed = 75.0f + (a_level * 20.0f);
	float fastEnemySpeed = normalEnemySpeed * 1.75f;

	for (int i = 0; i < enemyCount; i++) {
		Vector2 position{
			RandomFloat( 100.0f, static_cast<float>(renderer.GetWidth() - 100)),
			RandomFloat(100.0f, static_cast<float>(renderer.GetHeight() - 100))
		};

		if (a_level >= 2 && i % 3 == 0) {
			AddFastEnemy(position, fastEnemySpeed
			);
		}
		else {
			AddEnemy(position, normalEnemySpeed
			);
		}
	}
}

void FishGame::UpdateHUDText() {
	std::string hud = "Score: " + std::to_string(a_score) + " | High Score: " + std::to_string(a_highScore) + " | Lives: " + std::to_string(a_lives) + " | Level: " + std::to_string(a_level);
	a_hudText.Create(engine.GetRenderer(), hud, Color{ 1.0f, 1.0f, 1.0f });
}

void FishGame::LoadHighScore() {
	std::string data;

	if (!ReadTextFile("highscore.txt", data)) {
		a_highScore = 0;
		return;
	}

	try {
		a_highScore = std::stoi(data);
	}
	catch (...) {
		a_highScore = 0;
	}
}

void FishGame::SaveHighScore() {
	WriteTextFile("highscore.txt", std::to_string(a_highScore), false);
}

void FishGame::DrawPhysicsDemo(const Renderer& renderer) {
	Physics& physics = engine.GetPhysics();

	const b2BodyId* bodies = physics.GetDemoBodies();
	int bodyCount = physics.GetDemoBodyCount();

	const float pixelsPerMeter = 05.0f;
	
	// center
	const float screenCenterX = renderer.GetWidth() * 0.5f;
	const float screenCenterY = renderer.GetHeight() * 0.5f;

	// ground
	renderer.SetColor(120, 120, 120, 255);

	renderer.DrawFillRect(screenCenterX - (10.0f * pixelsPerMeter), screenCenterY + (5.0f * pixelsPerMeter), 20.0f * pixelsPerMeter, 1.0f * pixelsPerMeter);

	// falling boxes
	renderer.SetColor( 80, 180, 255, 255 );

	for (int i = 0; i < bodyCount; i++) {
		if (!b2Body_IsValid(bodies[i])) {
			continue;
		}

		b2Vec2 position = b2Body_GetPosition(bodies[i]);
		float screenX = screenCenterX + (position.x * pixelsPerMeter);
		float screenY = screenCenterY - (position.y * pixelsPerMeter);
		renderer.DrawFillRect( screenX - pixelsPerMeter, screenY - pixelsPerMeter, pixelsPerMeter * 2.0f, pixelsPerMeter * 2.0f );
	}
}

void FishGame::Draw(const Renderer& renderer) {
	renderer.SetColor(0, 0, 0, 255);
	renderer.Clear();

	switch (a_gameState) {
	case GameState::Title:
	case GameState::StartGame:
		a_stateText.Draw(renderer, 250.0f, 475.0f);
		break;

	case GameState::StartLevel:
		engine.GetPS().Draw(renderer);

		a_stateText.Draw(
			renderer,
			800.0f,
			500.0f
		);
		break;

	case GameState::Game:
		if (a_backgroundTexture != nullptr) {
			float screenW = static_cast<float>(renderer.GetWidth());
			float screenH = static_cast<float>(renderer.GetHeight());
			Vector2 texSize = a_backgroundTexture->GetSize();
			float scale = 1.0f;

			if (texSize.x > 0.0f && texSize.y > 0.0f) {
				float sx = screenW / texSize.x;
				float sy = screenH / texSize.y;
				scale = std::max(sx, sy);
			}

			Transform bgTransform{
				Vector2{ screenW * 0.5f, screenH * 0.5f },
				0.0f,
				1.0f
			};

			renderer.DrawTexture(
				*a_backgroundTexture,
				bgTransform,
				scale
			);
		}

		renderer.SetColor(
			255,
			255,
			255,
			255
		);


		Game::Draw(renderer);

		DrawPhysicsDemo(renderer);

		engine.GetPS().Draw(renderer);

		a_hudText.Draw(
			renderer,
			20.0f,
			20.0f
		);
		break;

	case GameState::GameOver:
		engine.GetPS().Draw(renderer);

		a_stateText.Draw(
			renderer,
			250.0f,
			475.0f
		);
		break;

	case GameState::Win:
		engine.GetPS().Draw(renderer);
		a_stateText.Draw(renderer, 250.0f, 475.0f);
		break;

	default:
		break;
	}

	renderer.Present();
}

void FishGame::Shutdown() {
	a_scene->RemoveAll();
	a_player = nullptr;
	a_texture.reset();
	a_stateText.SetFont(nullptr);
	a_hudText.SetFont(nullptr);
	a_font.reset();

	Game::Shutdown();
}