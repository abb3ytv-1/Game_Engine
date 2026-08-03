#pragma once
#include "Vector2.h"
#include "Vector3.h"

#include <vector>

namespace nu {
	class Mesh {
		public:
			Mesh() = default;

			Mesh(const std::vector<Vector2>& points, const Color& color) :
				a_points{points},
				a_color{color}
			{}

			void SetPoints(const std::vector<Vector2>& points) { a_points = points; }
			const std::vector<Vector2>& GetPoints() const { return a_points; }

			void SetColor(const Color& color) { a_color = color; }
			const Color& GetColor() const { return a_color; }

		private:
			std::vector<Vector2> a_points;
			Color a_color{ 1.0f, 1.0f, 1.0f };

	};
}