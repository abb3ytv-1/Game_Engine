#include "Physics.h"

namespace nu
{
    bool Physics::Initialize() {
        b2WorldDef worldDef = b2DefaultWorldDef();

        worldDef.gravity = b2Vec2{ 0.0f, -10.0f };

        a_worldId = b2CreateWorld(&worldDef);

        return true;
    }

    void Physics::Shutdown() {
        b2DestroyWorld(a_worldId);
    }

    void Physics::Update(float dt) {
        b2World_Step(a_worldId, dt, 4);
    }
}