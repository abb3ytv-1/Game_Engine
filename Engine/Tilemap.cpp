#include "pch.h"

#include "Tilemap.h"
#include "Renderer/Renderer.h"
#include "./Resources/Resource.h"

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

            layer.name = jsonLayer["name"].GetString();
            layer.width = jsonLayer["width"].GetInt();
            layer.height = jsonLayer["height"].GetInt();

            for (const auto& tile : jsonLayer["data"].GetArray())
            {
                layer.tiles.push_back(tile.GetInt());
            }

            a_layers.push_back(std::move(layer));
        }

        return true;
    }

}