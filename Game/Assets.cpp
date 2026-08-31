#include "Assets.h"

#include "../Engine/Renderer/Mesh.h"
#include "../Engine/Math/Vector2.h"
#include "../Engine/Math/Vector3.h"

#include <vector>

using namespace nu;

namespace {
	Model CreateFishModel(
		const Color& bodyColor,
		const Color& tailColor,
		const Color& topFinColor,
		const Color& sideFinColor,
		const Color& eyeColor
	) {
		std::vector<Vector2> bodyPoints{
			Vector2{ -6.0f, 0.0f },
			Vector2{ -3.0f, -3.0f },
			Vector2{ 3.0f, -3.0f },
			Vector2{ 7.0f, 0.0f },
			Vector2{ 3.0f, 3.0f },
			Vector2{ -3.0f, 3.0f },
			Vector2{ -6.0f, 0.0f }
		};

		std::vector<Vector2> tailPoints{
			Vector2{ -6.0f, 0.0f },
			Vector2{ -10.0f, -4.0f },
			Vector2{ -9.0f, 0.0f },
			Vector2{ -10.0f, 4.0f },
			Vector2{ -6.0f, 0.0f }
		};

		std::vector<Vector2> topFinPoints{
			Vector2{ -2.0f, -3.0f },
			Vector2{ 0.0f, -6.0f },
			Vector2{ 2.0f, -3.0f },
			Vector2{ -2.0f, -3.0f }
		};

		std::vector<Vector2> sideFinPoints{
			Vector2{ 0.0f, 1.0f },
			Vector2{ 2.0f, 4.0f },
			Vector2{ 3.0f, 1.0f },
			Vector2{ 0.0f, 1.0f }
		};

		std::vector<Vector2> eyePoints{
			Vector2{ 4.0f, -1.0f },
			Vector2{ 5.0f, -1.0f },
			Vector2{ 5.0f, 0.0f },
			Vector2{ 4.0f, 0.0f },
			Vector2{ 4.0f, -1.0f }
		};

		Mesh bodyMesh{
			bodyPoints,
			bodyColor
		};

		Mesh tailMesh{
			tailPoints,
			tailColor
		};

		Mesh topFinMesh{
			topFinPoints,
			topFinColor
		};

		Mesh sideFinMesh{
			sideFinPoints,
			sideFinColor
		};

		Mesh eyeMesh{
			eyePoints,
			eyeColor
		};

		Model model;

		model.AddMesh(bodyMesh);
		model.AddMesh(tailMesh);
		model.AddMesh(topFinMesh);
		model.AddMesh(sideFinMesh);
		model.AddMesh(eyeMesh);

		return model;
	}
}

Model nu::CreatePlayerModel() {
	// Return an empty model so no fish geometry is drawn. Visuals are handled by sprites.
	return Model();
}

Model nu::CreateEnemyModel() {
	// Return empty model to avoid drawing fish shapes.
	return Model();
}

Model nu::CreateFastEnemyModel() {
	// No fish geometry for fast enemy either.
	return Model();
}

Model nu::CreateBulletModel() {
	std::vector<Vector2> bulletPoints{
		Vector2{ -1.0f, -1.0f },
		Vector2{ 1.0f, 0.0f },
		Vector2{ -1.0f, 1.0f },
		Vector2{ -1.0f, -1.0f }
	};

	Mesh bulletMesh{
		bulletPoints,
		Color{ 1.0f, 0.0f, 0.0f }
	};

	Model bulletModel;

	bulletModel.AddMesh(bulletMesh);

	return bulletModel;
}