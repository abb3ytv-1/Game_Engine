#pragma once

#include "Framework/Component.h"
#include "Tilemap.h"

#include <memory>
#include <string>

namespace nu {

    class Renderer;
    class Actor;

    class TilemapRendererComponent : public Component {
    public:
        TilemapRendererComponent() = default;
        ~TilemapRendererComponent() override = default;

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
        std::string a_tilemapName;
        mutable res_t<Tilemap> a_tilemap;
    };

}