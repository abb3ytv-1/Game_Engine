#include "pch.h"

#include "Tilemap.h"
#include "Renderer/Renderer.h"
#include "Resources/ResourceManager.h"

#include <rapidjson/document.h>

#include <fstream>
#include <sstream>

namespace nu {

    bool Tilemap::Load(
        const std::string& filename,
        Renderer& renderer
    )
    {
        std::ifstream file(filename);

        if (!file.is_open())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        rapidjson::Document document;
        document.Parse(buffer.str().c_str());

        if (document.HasParseError())
        {
            return false;
        }

        if (!document.HasMember("width") ||
            !document.HasMember("height") ||
            !document.HasMember("tilewidth") ||
            !document.HasMember("tileheight"))
        {
            return false;
        }

        a_width = document["width"].GetInt();
        a_height = document["height"].GetInt();

        a_tileWidth = document["tilewidth"].GetInt();
        a_tileHeight = document["tileheight"].GetInt();

        a_layers.clear();

        if (!document.HasMember("layers") ||
            !document["layers"].IsArray())
        {
            return false;
        }

        for (const auto& jsonLayer : document["layers"].GetArray())
        {
            if (!jsonLayer.HasMember("name") ||
                !jsonLayer.HasMember("width") ||
                !jsonLayer.HasMember("height") ||
                !jsonLayer.HasMember("data"))
            {
                continue;
            }

            TilemapLayer layer;

            layer.name =
                jsonLayer["name"].GetString();

            layer.width =
                jsonLayer["width"].GetInt();

            layer.height =
                jsonLayer["height"].GetInt();

            for (const auto& tile :
                jsonLayer["data"].GetArray())
            {
                layer.tiles.push_back(
                    tile.GetInt()
                );
            }

            a_layers.push_back(
                std::move(layer)
            );
        }

        // Find the texture_name property in the map.
        for (const auto& jsonLayer :
            document["layers"].GetArray())
        {
            if (!jsonLayer.HasMember("properties") ||
                !jsonLayer["properties"].IsArray())
            {
                continue;
            }

            for (const auto& property :
                jsonLayer["properties"].GetArray())
            {
                if (!property.HasMember("name") ||
                    !property.HasMember("value"))
                {
                    continue;
                }

                const std::string propertyName =
                    property["name"].GetString();

                if (propertyName == "texture_name")
                {
                    const std::string textureName =
                        property["value"].GetString();

                    a_tilesetTexture =
                        Resources().Get<Texture>(
                            textureName,
                            renderer
                        );

                    break;
                }
            }

            if (a_tilesetTexture != nullptr)
            {
                break;
            }
        }

        if (a_tilesetTexture == nullptr)
        {
            return false;
        }

        const Vector2 textureSize =
            a_tilesetTexture->GetSize();

        a_tilesetColumns =
            static_cast<int>(
                textureSize.x
                ) / a_tileWidth;

        if (a_tilesetColumns <= 0)
        {
            return false;
        }

        return true;
    }

}