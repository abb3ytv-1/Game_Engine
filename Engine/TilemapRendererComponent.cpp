#include "pch.h"

#include "TilemapRendererComponent.h"

#include "Framework/Actor.h"
#include "./Renderer/Renderer.h"
#include "Resources/ResourceManager.h"

namespace nu {

    std::unique_ptr<Component> TilemapRendererComponent::Clone() const
    {
        auto component = std::make_unique<TilemapRendererComponent>();

        component->a_tilemapName = a_tilemapName;
        component->a_tilemap = a_tilemap;

        return component;
    }

    void TilemapRendererComponent::OnAttach(Actor& actor)
    {
        // The renderer is not available when a component is attached.
        // The tilemap will be loaded when Draw() receives the renderer.
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

        if (a_tilemap == nullptr)
        {
            return;
        }

        if (a_tilemap->GetTilesetTexture() == nullptr)
        {
            return;
        }

        if (a_tilemap->GetTilesetColumns() <= 0)
        {
            return;
        }

        const auto& layers = a_tilemap->GetLayers();

        for (const auto& layer : layers)
        {
            // Prevent division/modulo by zero below.
            if (layer.width <= 0 || layer.height <= 0)
            {
                continue;
            }

            for (int i = 0; i < static_cast<int>(layer.tiles.size()); ++i)
            {
                const int tileId = layer.tiles[i];

                // Tiled uses 0 for an empty tile.
                if (tileId <= 0)
                {
                    continue;
                }

                const int column =
                    (tileId - 1) % a_tilemap->GetTilesetColumns();

                const int row =
                    (tileId - 1) / a_tilemap->GetTilesetColumns();

                SDL_FRect sourceRect{
                    static_cast<float>(
                        column * a_tilemap->GetTileWidth()
                    ),
                    static_cast<float>(
                        row * a_tilemap->GetTileHeight()
                    ),
                    static_cast<float>(
                        a_tilemap->GetTileWidth()
                    ),
                    static_cast<float>(
                        a_tilemap->GetTileHeight()
                    )
                };

                const int tileX = i % layer.width;
                const int tileY = i / layer.width;

                Transform transform;

                transform.position = Vector2{
                    static_cast<float>(
                        tileX * a_tilemap->GetTileWidth()
                    ),
                    static_cast<float>(
                        tileY * a_tilemap->GetTileHeight()
                    )
                };

                renderer.DrawTexture(
                    *a_tilemap->GetTilesetTexture(),
                    transform,
                    sourceRect
                );
            }
        }
    }

}