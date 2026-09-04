#include "Player.h"
#include "../Engine/Core/Factory.h"

using namespace nu;

Player::Player(
    const Transform& transform,
    const Model& model,
    float speed
) :
    Actor{ transform, model },
    a_speed{ speed }
{}

void nu::RegisterPlayer()
{
    nu::Factory::Instance().Register(
        "Player",
        std::make_unique<nu::Creator<nu::Player>>()
    );
}