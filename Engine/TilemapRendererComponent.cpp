#include "pch.h"

#include "TilemapRendererComponent.h"

#include "Framework/Actor.h"
#include "./Renderer/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Engine.h"

namespace nu {

    TilemapRendererComponent::~TilemapRendererComponent()
    {
        for (b2BodyId bodyId : a_collisionBodies) {
            if (b2Body_IsValid(bodyId)) {
                b2DestroyBody(bodyId);
            }
        }
    }

    std::unique_ptr<Component> TilemapRendererComponent::Clone() const
    {
        auto component = std::make_unique<TilemapRendererComponent>();

        component->a_tilemapName = a_tilemapName;
        component->a_tilemap = a_tilemap;
        // Deliberately NOT copying a_collisionBodies/a_bodiesCreated -
        // the clone creates its own bodies on its own first Draw().

        return component;
    }

    void TilemapRendererComponent::OnAttach(Actor& actor)
    {
        // The renderer is not available when a component is attached.
        // The tilemap will be loaded when Draw() receives the renderer.
    }

    void TilemapRendererComponent::CreateCollisionBodies(const Actor& actor) const
    {
        if (a_bodiesCreated) {
            return;
        }

        a_bodiesCreated = true;

        if (a_tilemap == nullptr) {
            return;
        }

        b2WorldId worldId = engine.GetPhysics().GetWorldId();

        if (!b2World_IsValid(worldId)) {
            return;
        }

        const Transform& mapTransform = actor.GetTransform();
        const Vector2& origin = mapTransform.position;
        const float mapScale = mapTransform.scale;

        
        const float pixelsPerMeter =
            static_cast<float>(a_tilemap->GetTileWidth());

        const auto& layers = a_tilemap->GetLayers();

        for (const auto& layer : layers) {
            if (!layer.hasCollision) {
                continue;
            }

            if (layer.width <= 0 || layer.height <= 0) {
                continue;
            }

            for (int i = 0; i < static_cast<int>(layer.tiles.size()); ++i) {
                if (layer.tiles[i] <= 0) {
                    continue; 
                }

                const int tileX = i % layer.width;
                const int tileY = i / layer.width;

                Vector2 tileCenterPixels = origin + (Vector2{
                    static_cast<float>(tileX * a_tilemap->GetTileWidth()) +
                        (a_tilemap->GetTileWidth() * 0.5f),
                    static_cast<float>(tileY * a_tilemap->GetTileHeight()) +
                        (a_tilemap->GetTileHeight() * 0.5f)
                    } * mapScale);

                float halfWidthMeters =
                    (a_tilemap->GetTileWidth() * mapScale * 0.5f) / pixelsPerMeter;

                float halfHeightMeters =
                    (a_tilemap->GetTileHeight() * mapScale * 0.5f) / pixelsPerMeter;

                b2BodyDef bodyDef = b2DefaultBodyDef();
                bodyDef.type = b2_staticBody;
                bodyDef.position = b2Vec2{
                    tileCenterPixels.x / pixelsPerMeter,
                    -(tileCenterPixels.y / pixelsPerMeter) 
                };

                b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

                b2Polygon box = b2MakeBox(halfWidthMeters, halfHeightMeters);
                b2ShapeDef shapeDef = b2DefaultShapeDef();

                b2CreatePolygonShape(bodyId, &shapeDef, &box);

                a_collisionBodies.push_back(bodyId);
            }
        }
    }

    void TilemapRendererComponent::Draw(
        const Renderer& renderer,
        const Actor& actor
    ) const
    {
        if (a_tilemap == nullptr && !a_tilemapName.empty())
        {
            a_tilemap = Resources().Get<Tilemap>(
                a_tilemapName,
                const_cast<Renderer&>(renderer)
            );
        }

        if (a_tilemap == nullptr) return;
        if (a_tilemap->GetTilesetTexture() == nullptr) return;
        if (a_tilemap->GetTilesetColumns() <= 0) return;

        CreateCollisionBodies(actor);

        const Transform& mapTransform = actor.GetTransform();
        const Vector2& origin = mapTransform.position;
        const float mapScale = mapTransform.scale;

        const auto& layers = a_tilemap->GetLayers();

        for (const auto& layer : layers)
        {
            if (layer.width <= 0 || layer.height <= 0) continue;

            for (int i = 0; i < static_cast<int>(layer.tiles.size()); ++i)
            {
                const int tileId = layer.tiles[i];
                if (tileId <= 0) continue;

                const int column = (tileId - 1) % a_tilemap->GetTilesetColumns();
                const int row = (tileId - 1) / a_tilemap->GetTilesetColumns();

                SDL_FRect sourceRect{
                    static_cast<float>(column * a_tilemap->GetTileWidth()),
                    static_cast<float>(row * a_tilemap->GetTileHeight()),
                    static_cast<float>(a_tilemap->GetTileWidth()),
                    static_cast<float>(a_tilemap->GetTileHeight())
                };

                const int tileX = i % layer.width;
                const int tileY = i / layer.width;

                Transform tileTransform;
                tileTransform.position = origin + (Vector2{
                    static_cast<float>(tileX * a_tilemap->GetTileWidth()),
                    static_cast<float>(tileY * a_tilemap->GetTileHeight())
                    } * mapScale);

                renderer.DrawTexture(
                    *a_tilemap->GetTilesetTexture(),
                    tileTransform,
                    sourceRect,
                    mapScale,
                    Vector2{ 0.0f, 0.0f }   
                );
            }
        }
    }

}