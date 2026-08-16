#include "Player.h"
#include "../Engine/Factory.h"

using namespace nu;

Player::Player(
	const Transform& transform,
	const Model& model,
	float speed
) :
	Actor{ transform, model },
	a_speed{ speed }
{}