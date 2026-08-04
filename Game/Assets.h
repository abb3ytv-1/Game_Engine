#pragma once
#include <memory>
#include "../Engine/Model.h"

namespace nu {
	std::shared_ptr<Model> CreatePlayerModel();
	std::shared_ptr<Model> CreateEnemyModel();
	std::shared_ptr<Model> CreateFastEnemyModel();
	std::shared_ptr<Model> CreateBulletModel();
}