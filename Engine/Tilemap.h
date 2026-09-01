#pragma once

#include "Resources/Resource.h"
#include "Renderer/Texture.h"

#include <string>
#include <vector>

namespace nu {

	struct TilemapLayer {
		std::string name;
		int width{ 0 };
		int height{ 0 };
		std::vector<int> tiles;
	};

	class Tilemap : public Resource {
	public:
		Tilemap() = default;

		bool Load(
			const std::string& filename,
			Renderer& renderer
		);

		int GetWidth() const {
			return a_width;
		}

		int GetHeight() const {
			return a_height;
		}

		int GetTileWidth() const {
			return a_tileWidth;
		}

		int GetTileHeight() const {
			return a_tileHeight;
		}

		const std::vector<TilemapLayer>& GetLayers() const {
			return a_layers;
		}

		res_t<Texture> GetTilesetTexture() const {
			return a_tilesetTexture;
		}

		int GetTilesetColumns() const {
			return a_tilesetColumns;
		}

	private:
		int a_width{ 0 };
		int a_height{ 0 };

		int a_tileWidth{ 0 };
		int a_tileHeight{ 0 };

		int a_tilesetColumns{ 0 };

		res_t<Texture> a_tilesetTexture;

		std::vector<TilemapLayer> a_layers;
	};

}