#include "pch.h"
#include "Physics.h"

namespace nu
{
    bool Physics::Initialize() {
        b2WorldDef worldDef = b2DefaultWorldDef();

        worldDef.gravity = b2Vec2{ 0.0f, -10.0f };

        a_worldId = b2CreateWorld(&worldDef);

        // GROUND
        b2BodyDef groundBodyDef = b2DefaultBodyDef();
        groundBodyDef.position = b2Vec2{ 0.0f, -5.0f };

        b2BodyId groundBody = b2CreateBody(a_worldId, &groundBodyDef);
        b2Polygon groundBox = b2MakeBox(10.0f, 0.05f);
        b2ShapeDef groundShapeDef = b2DefaultShapeDef();

        b2CreatePolygonShape(groundBody, &groundShapeDef, &groundBox);


        // Falling boxes
        for (int i = 0; i < 3; i++) {
            b2BodyDef dynamicBodyDef = b2DefaultBodyDef();

            dynamicBodyDef.type = b2_dynamicBody;
            dynamicBodyDef.position = b2Vec2{ -3.0f + (i * 3.0f), 5.0f + (i * 2.0f) };

            b2BodyId dynamicBody = b2CreateBody( a_worldId, &dynamicBodyDef );

            b2Polygon box = b2MakeBox(1.0f, 1.0f);

            b2ShapeDef boxShapeDef = b2DefaultShapeDef();

            boxShapeDef.density = 1.0f;
            boxShapeDef.material.friction = 0.3f;
            boxShapeDef.material.restitution = 0.5f;

            b2CreatePolygonShape( dynamicBody, &boxShapeDef, &box );
        }

        return true;
    }

    void Physics::Shutdown() {
        if (b2World_IsValid(a_worldId)) {
            b2DestroyWorld(a_worldId);
        }
    }

    void Physics::Update(float dt) {
        if (b2World_IsValid(a_worldId)) {
            b2World_Step(a_worldId, dt, 4);
        }
    }
}