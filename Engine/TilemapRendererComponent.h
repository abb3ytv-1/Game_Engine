#pragma once

#include "Framework/Component.h"
#include "Tilemap.h"

#include <box2d/box2d.h>

#include <memory>
#include <string>
#include <vector>

namespace nu {

    class Renderer;
    class Actor;

    class TilemapRendererComponent : public Component {
    public:
        TilemapRendererComponent() = default;
        ~TilemapRendererComponent() override;  

        std::unique_ptr<Component> Clone() const override;

        void OnAttach(Actor& actor) override;

        void Draw(
            const Renderer& renderer,
            const Actor& actor
        ) const override;

        void SetTilemapName(const std::string& name) {
            a_tilemapName = name;
        }

    private:
        void CreateCollisionBodies(const Actor& actor) const;  

    private:
        std::string a_tilemapName;
        mutable res_t<Tilemap> a_tilemap;

        mutable bool a_bodiesCreated{ false };              
        mutable std::vector<b2BodyId> a_collisionBodies;    
    };

}