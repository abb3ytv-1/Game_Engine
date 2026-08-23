#include "Space_Game.h"

#include "../Engine/PlayerComponent.h"
#include "../Engine/EnemyAIComponent.h"
#include "../Engine/BulletComponent.h"
#include "../Engine/SpriteRendererComponent.h"
#include "../Engine/CircleCollider2DComponent.h"
#include "../Engine/RigidBodyComponent.h"
#include "../Engine/Engine.h"
#include "../Engine/File.h"
#include "../Engine/MathUtils.h"
#include "../Engine/ParticleSystem.h"
#include "../Engine/Random.h"

#include "Assets.h"
#include "../Engine/Factory.h"

#include <SDL3/SDL.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <filesystem>
#include <algorithm>

using namespace nu;

SpaceGame::SpaceGame() = default;

int SpaceGame::Run() {
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

bool SpaceGame::Initialize() {
	if (!Game::Initialize()) {
		return false;
	}

	a_scene = &a_gameScene;

	if (!SetWorkingDirectory("Assets")) {
		std::cerr
			<< "Could not set Assets working directory.\n";

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

	// Load the shared player texture resource (fall back to existing if missing)
	a_texture = Resources().Get<Texture>(
		"Textures/large_grey_01.png",
		engine.GetRenderer()
	);

	if (a_texture == nullptr) {
		// fallback to previous sample texture
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

	// Request the same texture again to confirm
	// that the Resource Manager reuses it.
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

	// Load sprite textures (optional - fall back to models if missing)
	a_enemyTexture = Resources().Get<Texture>(
		"Textures/large_grey_02.png",
		engine.GetRenderer()
	);

	a_fastEnemyTexture = Resources().Get<Texture>(
		"Textures/large_purple_01.png",
		engine.GetRenderer()
	);

	a_bulletTexture = Resources().Get<Texture>(
		"Textures/missile-2.png",
		engine.GetRenderer()
	);

	a_backgroundTexture = Resources().Get<Texture>(
		"Textures/background.png",
		engine.GetRenderer()
	);

	a_particleTexture = Resources().Get<Texture>(
		"Textures/spark_06.png",
		engine.GetRenderer()
	);

	// Provide particle texture to the particle system if available
	if (a_particleTexture != nullptr) {
		engine.GetPS().SetTexture(a_particleTexture);
	}

	a_gameState = GameState::StartGame;

	return true;
}

bool SpaceGame::LoadAudio() {
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

void SpaceGame::CreateActors() {
	// Create a plain Actor and compose it with components for player behavior
	std::unique_ptr<nu::Actor> playerActor = std::make_unique<nu::Actor>();
	playerActor->SetTransform(Transform{ Vector2{960.0f, 540.0f}, 0.0f, 10.0f });
	playerActor->a_model = a_playerModel;

	// Attach PlayerComponent with default speed
	playerActor->AddComponent(std::make_unique<nu::PlayerComponent>(300.0f, 0));
	// Attach rigid body for movement
	playerActor->AddComponent(std::make_unique<nu::RigidBodyComponent>());

	// Attach collider and renderer will be configured below if textures exist

	// Set a_player pointer to the raw actor for input handling
	a_player = playerActor.get();

	// Assign texture and compute a texture scale so the sprite matches the
	// actor's collision radius in world units (width ~= 2 * collision radius).
	// First set the collision radius, then compute scale from the texture size.
	if (a_player != nullptr) {
		a_player->SetCollisionRadius(8.0f);
		if (a_texture != nullptr) {
			Vector2 texSize = a_texture->GetSize();
			if (texSize.x > 0.0f) {
				float desiredDiameter = a_player->GetCollisionRadius() * 2.0f;
				float scale = desiredDiameter / texSize.x;
				// clamp to reasonable range
				scale = std::clamp(scale, 0.01f, 5.0f);
				a_player->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_texture, scale, Vector2{0.5f, 0.0f}));
			}
			else {
				a_player->AddComponent(std::make_unique<nu::SpriteRendererComponent>(a_texture, 1.0f, Vector2{0.5f,0.0f}));
			}
		}
	}

	// If an enemy texture is available, we'll use it for spawned enemies later.

	a_gameScene.AddActor(
		std::move(playerActor)
	);

	AddEnemy(
		Vector2{ 200.0f, 200.0f },
		100.0f
	);

	AddEnemy(
		Vector2{ 1700.0f, 300.0f },
		125.0f
	);

	AddEnemy(
		Vector2{ 400.0f, 900.0f },
		75.0f
	);
}

void SpaceGame::AddEnemy(
	const Vector2& position,
	float speed
) {
	if (a_player == nullptr) {
		return;
	}

	// Compose a plain Actor for the enemy
	auto enemy = std::make_unique<nu::Actor>();
	enemy->SetTransform(Transform{ position, 0.0f, 8.0f });
	enemy->a_model = a_enemyModel;

	// Attach EnemyAIComponent and set target & speed
	enemy->AddComponent(std::make_unique<nu::EnemyAIComponent>(a_player, speed));
	// Attach rigid body so enemy moves
	enemy->AddComponent(std::make_unique<nu::RigidBodyComponent>());
	enemy->SetCollisionRadius(8.0f);

	// If an enemy sprite is loaded, use it and auto-scale to match collision radius
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


	a_gameScene.AddActor(
		std::move(enemy)
	);
}

void SpaceGame::AddFastEnemy(
	const Vector2& position,
	float speed
) {
	if (a_player == nullptr) {
		return;
	}

	// Compose a plain Actor for the fast enemy
	auto enemy = std::make_unique<nu::Actor>();
	enemy->SetTransform(Transform{ position, 0.0f, 6.0f });
	enemy->a_model = a_fastEnemyModel;
	enemy->AddComponent(std::make_unique<nu::EnemyAIComponent>(a_player, speed));
	enemy->AddComponent(std::make_unique<nu::RigidBodyComponent>());
	enemy->SetCollisionRadius(6.0f);

	// Prefer fast-specific sprite if available, otherwise fall back to enemy texture
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

	a_gameScene.AddActor(
		std::move(enemy)
	);
}

void SpaceGame::ProcessEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			a_quit = true;
			continue;
		}

		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (
				event.key.scancode ==
				SDL_SCANCODE_ESCAPE
				) {
				a_quit = true;
				continue;
			}

			bool enterPressed =
				event.key.scancode ==
				SDL_SCANCODE_RETURN ||
				event.key.scancode ==
				SDL_SCANCODE_KP_ENTER;

			bool canStartGame =
				a_gameState ==
				GameState::Title ||
				a_gameState ==
				GameState::StartGame ||
				a_gameState ==
				GameState::GameOver;

			if (enterPressed && canStartGame) {
				StartNewGame();
			}
		}
	}
}

void SpaceGame::Update(float dt) {
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
		EmitPlayerParticle();
		HandleMouseInput();

		Game::Update(dt);

		CheckCollisions();
		break;

	case GameState::GameOver:
		break;

	default:
		break;
	}
}

void SpaceGame::HandleAudioInput() {
	Input& input = engine.GetInput();
	Audio& audio = engine.GetAudio();

	if (input.GetKeyPress(SDL_SCANCODE_1)) {
		audio.PlaySound("bass");
	}

	if (input.GetKeyPress(SDL_SCANCODE_2)) {
		audio.PlaySound("snare");
	}

	if (input.GetKeyPress(SDL_SCANCODE_3)) {
		audio.PlaySound("clap");
	}

	if (input.GetKeyPress(SDL_SCANCODE_4)) {
		audio.PlaySound("open-hat");
	}

	if (input.GetKeyPress(SDL_SCANCODE_5)) {
		audio.PlaySound("cowbell");
	}
}

void SpaceGame::HandlePlayerInput(float dt) {
	if (
		a_player == nullptr ||
		a_player->IsDestroyed()
		) {
		return;
	}

	Input& input = engine.GetInput();

	float rotation =
		a_player->GetTransform().rotation;

	if (input.GetKeyDown(SDL_SCANCODE_LEFT)) {
		rotation -= a_rotationSpeed * dt;
	}

	if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) {
		rotation += a_rotationSpeed * dt;
	}

	a_player->SetRotation(rotation);

	Vector2 direction{ 0.0f, 0.0f };

	if (input.GetKeyDown(SDL_SCANCODE_W)) {
		direction.y -= 1.0f;
	}

	if (input.GetKeyDown(SDL_SCANCODE_S)) {
		direction.y += 1.0f;
	}

	if (input.GetKeyDown(SDL_SCANCODE_A)) {
		direction.x -= 1.0f;
	}

	if (input.GetKeyDown(SDL_SCANCODE_D)) {
		direction.x += 1.0f;
	}

	if (direction.LengthSqr() > 0.0f) {
		direction = direction.Normalized();
	}

	// Get player speed from PlayerComponent
	if (a_player) {
		auto* pc = a_player->GetComponent<nu::PlayerComponent>();
		float speed = pc ? pc->GetSpeed() : 0.0f;
		a_player->SetVelocity(direction * speed);
	}

	if (input.GetKeyPress(SDL_SCANCODE_SPACE)) {
		HandleShooting();
	}
}

void SpaceGame::HandleShooting() {
	if (
		a_player == nullptr ||
		a_player->IsDestroyed()
		) {
		return;
	}

	// Apply a sprite-forward rotation offset so bullets spawn from the
	// sprite's front regardless of how the artwork is oriented.
	float rotation =
		a_player->GetTransform().rotation +
		a_playerSpriteRotationOffsetDeg;

	Vector2 forward{ 1.0f, 0.0f };

	forward = forward.Rotate(
		rotation * DegToRad
	);

	float spawnDistance =
		a_player->GetCollisionRadius() +
		10.0f;

	Vector2 bulletPosition =
		a_player->GetTransform().position +
		(forward * spawnDistance);

	// Compose a plain Actor bullet with a BulletComponent tag
	std::unique_ptr<nu::Actor> bullet = std::make_unique<nu::Actor>();
	bullet->SetTransform(Transform{ bulletPosition, rotation, 4.0f });
	bullet->a_model = a_bulletModel;

	// Compute forward velocity like Bullet ctor
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

	a_gameScene.AddActor(
		std::move(bullet)
	);

	engine.GetAudio().PlaySound("snare");
}

void SpaceGame::HandleMouseInput() {
	Input& input = engine.GetInput();

	// Mouse-drawn shapes disabled per user request: do not record mouse points.
}

void SpaceGame::CheckCollisions() {
	auto& actors =
		a_gameScene.GetActors();

	// Bullet and enemy collisions
	for (auto& actor : actors) {
		// Identify bullets by tag component
		auto* bulletTag = actor->GetComponent<nu::BulletComponent>();
		if (bulletTag == nullptr || actor->IsDestroyed()) continue;

		for (auto& otherActor : actors) {
			auto* enemyAI = otherActor->GetComponent<nu::EnemyAIComponent>();
			if (enemyAI == nullptr || otherActor->IsDestroyed()) continue;

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

	// Player and enemy collisions
	if (a_player == nullptr || a_player->IsDestroyed() || a_playerInvincibilityTimer > 0.0f) {
		return;
	}

	bool gameOver = false;

	for (auto& actor : actors) {
		auto* enemyAI = actor->GetComponent<nu::EnemyAIComponent>();
		if (enemyAI == nullptr || actor->IsDestroyed()) continue;

		if (!a_player->IsColliding(*actor)) continue;

		Vector2 collisionPosition = actor->GetTransform().position;

		actor->Destroy();

		CreateExplosion(collisionPosition, Color{ 1.0f, 0.2f, 0.2f }, 75);

		a_lives--;

		engine.GetAudio().PlaySound("bass");

		UpdateHUDText();

		if (a_lives <= 0) { gameOver = true; break; }

		Renderer& renderer = engine.GetRenderer();

		a_player->SetPosition(Vector2{ renderer.GetWidth() * 0.5f, renderer.GetHeight() * 0.5f });
		a_player->SetVelocity(Vector2{ 0.0f, 0.0f });
		a_player->SetRotation(0.0f);
		a_playerInvincibilityTimer = 1.5f;
		break;
	}

	if (gameOver) {
		EndGame();
		return;
	}

	if (!HasActiveEnemies()) {
		StartNextLevel();
	}
}

void SpaceGame::CreateExplosion(
	const Vector2& position,
	const Color& color,
	int particleCount
) {
	ParticleSystem& particleSystem =
		engine.GetPS();

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

		// Visual size and rotation
		particle.size = RandomFloat(4.0f, 12.0f);
		particle.rotation = RandomFloat(0.0f, 360.0f);
		particle.angularVelocity = RandomFloat(-180.0f, 180.0f);

		particleSystem.AddParticle(particle);
	}
}

void SpaceGame::EmitPlayerParticle() {
	if (
		a_player == nullptr ||
		a_player->IsDestroyed()
		) {
		return;
	}

	if (
		a_player->GetVelocity().LengthSqr() <=
		0.0f
		) {
		return;
	}

	// Use the same sprite-forward offset so particle trail emits from the
	// visual rear of the sprite.
	float rotation =
		a_player->GetTransform().rotation +
		a_playerSpriteRotationOffsetDeg;

	Vector2 forward{ 1.0f, 0.0f };

	forward = forward.Rotate(
		rotation * DegToRad
	);

	float trailDistance =
		a_player->GetCollisionRadius() +
		20.0f;

	Particle particle;

	particle.position =
		a_player->GetTransform().position -
		(forward * trailDistance);

	particle.position.x +=
		RandomFloat(-5.0f, 5.0f);

	particle.position.y +=
		RandomFloat(-5.0f, 5.0f);

	particle.color =
		Color{ 0.4f, 0.8f, 1.0f };

	particle.lifespan =
		RandomFloat(0.25f, 0.75f);
	particle.velocity =
		(forward *
			RandomFloat(-100.0f, -40.0f)) +
		Vector2{
			RandomFloat(-30.0f, 30.0f),
			RandomFloat(-30.0f, 30.0f)
	};

	// Trail particle visual parameters
	particle.size = RandomFloat(2.0f, 6.0f);
	particle.rotation = RandomFloat(0.0f, 360.0f);
	particle.angularVelocity = RandomFloat(-120.0f, 120.0f);

	engine.GetPS().AddParticle(particle);
}

void SpaceGame::StartNewGame() {
	a_gameScene.RemoveAll();

	a_player = nullptr;
	a_score = 0;
	a_lives = 3;
	a_level = 1;
	a_levelStartTimer = 0.0f;
	a_playerInvincibilityTimer = 0.0f;

	a_mousePoints.clear();
	a_startsNewShape.clear();

	CreateActors();
	UpdateHUDText();

	a_gameState = GameState::Game;
}

void SpaceGame::EndGame() {
	engine.GetAudio().PlaySound("cowbell");

	a_gameScene.RemoveAll();
	a_player = nullptr;

	if (a_score > a_highScore) {
		a_highScore = a_score;
		SaveHighScore();
	}

	std::string message =
		"Game Over | Score: " +
		std::to_string(a_score) +
		" | High Score: " +
		std::to_string(a_highScore) +
		" | Press Enter to Play Again";

	a_stateText.Create(
		engine.GetRenderer(),
		message,
		Color{ 1.0f, 0.25f, 0.25f }
	);

	a_gameState = GameState::GameOver;
}

bool SpaceGame::HasActiveEnemies() const {
	for (const auto& actor : a_gameScene.GetActors()) {
		// Detect enemies by EnemyAIComponent so component-based actors are recognized
		auto* enemyAI = actor->GetComponent<nu::EnemyAIComponent>();
		if (enemyAI != nullptr && !actor->IsDestroyed()) {
			return true;
		}
	}
	return false;
}

void SpaceGame::StartNextLevel() {
	a_level++;
	a_levelStartTimer = 2.0f;

	std::string message =
		"Level " +
		std::to_string(a_level);

	a_stateText.Create(
		engine.GetRenderer(),
		message,
		Color{ 0.45f, 0.85f, 1.0f }
	);

	engine.GetAudio().PlaySound("open-hat");

	UpdateHUDText();

	a_gameState = GameState::StartLevel;
}

void SpaceGame::SpawnLevelEnemies() {
	Renderer& renderer =
		engine.GetRenderer();

	int enemyCount = a_level + 2;

	float normalEnemySpeed =
		75.0f +
		(a_level * 20.0f);

	float fastEnemySpeed =
		normalEnemySpeed * 1.75f;

	for (int i = 0; i < enemyCount; i++) {
		Vector2 position{
			RandomFloat(
				100.0f,
				static_cast<float>(
					renderer.GetWidth() - 100
				)
			),
			RandomFloat(
				100.0f,
				static_cast<float>(
					renderer.GetHeight() - 100
				)
			)
		};

		if (
			a_level >= 2 &&
			i % 3 == 0
			) {
			AddFastEnemy(
				position,
				fastEnemySpeed
			);
		}
		else {
			AddEnemy(
				position,
				normalEnemySpeed
			);
		}
	}
}

void SpaceGame::UpdateHUDText() {
	std::string hud =
		"Score: " +
		std::to_string(a_score) +
		" | High Score: " +
		std::to_string(a_highScore) +
		" | Lives: " +
		std::to_string(a_lives) +
		" | Level: " +
		std::to_string(a_level);

	a_hudText.Create(
		engine.GetRenderer(),
		hud,
		Color{ 1.0f, 1.0f, 1.0f }
	);
}

void SpaceGame::LoadHighScore() {
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

void SpaceGame::SaveHighScore() {
	WriteTextFile(
		"highscore.txt",
		std::to_string(a_highScore),
		false
	);
}

void SpaceGame::Draw(
	const Renderer& renderer
) {
	renderer.SetColor(0, 0, 0, 255);
	renderer.Clear();

	switch (a_gameState) {
	case GameState::Title:
	case GameState::StartGame:
		a_stateText.Draw(
			renderer,
			250.0f,
			475.0f
		);
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
		// Draw background texture if available (scaled to fill screen)
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

		// Mouse-drawn shapes disabled per user request — no rendering here.

		Game::Draw(renderer);

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

	default:
		break;
	}

	renderer.Present();
}

void SpaceGame::Shutdown() {
	a_gameScene.RemoveAll();
	a_player = nullptr;

	a_texture.reset();

	a_stateText.SetFont(nullptr);
	a_hudText.SetFont(nullptr);
	a_font.reset();

	Game::Shutdown();
}