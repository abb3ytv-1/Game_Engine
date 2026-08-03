#include "Player.h"

using namespace nu;

Player::Player(
	const Transform& transform,
	const Model& model,
	float speed
) :
	Actor{ transform, model },
	a_speed{ speed }
{}