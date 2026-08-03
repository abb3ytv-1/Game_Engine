#include "Assets.h"

#include "../Engine/Mesh.h"
#include "../Engine/Vector2.h"
#include "../Engine/Vector3.h"

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
	return CreateFishModel(
		Color{ 0.45f, 0.85f, 0.80f },
		Color{ 1.0f, 0.72f, 0.25f },
		Color{ 0.65f, 0.45f, 0.90f },
		Color{ 0.95f, 0.45f, 0.70f },
		Color{ 1.0f, 1.0f, 1.0f }
	);
}

Model nu::CreateEnemyModel() {
	return CreateFishModel(
		Color{ 1.0f, 0.2f, 0.2f },
		Color{ 0.7f, 0.0f, 0.0f },
		Color{ 0.8f, 0.1f, 0.1f },
		Color{ 0.6f, 0.0f, 0.0f },
		Color{ 0.0f, 0.0f, 0.0f }
	);
}

Model nu::CreateFastEnemyModel() {
	std::vector<Vector2> bodyPoints{
		Vector2{ -5.0f, 0.0f },
		Vector2{ -2.0f, -2.0f },
		Vector2{ 3.0f, -2.0f },
		Vector2{ 7.0f, 0.0f },
		Vector2{ 3.0f, 2.0f },
		Vector2{ -2.0f, 2.0f },
		Vector2{ -5.0f, 0.0f }
	};

	std::vector<Vector2> tailPoints{
		Vector2{ -5.0f, 0.0f },
		Vector2{ -8.0f, -3.0f },
		Vector2{ -7.0f, 0.0f },
		Vector2{ -8.0f, 3.0f },
		Vector2{ -5.0f, 0.0f }
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
		Color{ 0.75f, 0.2f, 1.0f }
	};

	Mesh tailMesh{
		tailPoints,
		Color{ 1.0f, 0.25f, 0.75f }
	};

	Mesh eyeMesh{
		eyePoints,
		Color{ 1.0f, 1.0f, 1.0f }
	};

	Model model;

	model.AddMesh(bodyMesh);
	model.AddMesh(tailMesh);
	model.AddMesh(eyeMesh);

	return model;
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