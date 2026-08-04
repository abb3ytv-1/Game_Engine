#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

namespace nu {
	class Mesh;
	class Model;
	struct Transform;

	class Renderer {
	public:
		bool Initialize(
			const char* name,
			int width,
			int height
		);

		void Shutdown();

		void Clear() const;
		void Present() const;

		void SetColor(
			Uint8 r,
			Uint8 g,
			Uint8 b,
			Uint8 a = 255
		) const;

		void DrawPoint(float x, float y) const;

		void DrawFillRect(
			float x,
			float y,
			float width,
			float height
		) const;

		void DrawRect(
			float x,
			float y,
			float width,
			float height
		) const;

		void DrawLine(
			float x1,
			float y1,
			float x2,
			float y2
		) const;

		void DrawMesh(
			const Mesh& mesh,
			const Transform& transform
		) const;

		void DrawModel(
			const Model& model,
			const Transform& transform
		) const;

		int GetWidth() const {
			return a_width;
		}

		int GetHeight() const {
			return a_height;
		}

	private:
		friend class Text;
		friend class Texture;

		SDL_Window* a_window{ nullptr };
		SDL_Renderer* a_renderer{ nullptr };

		int a_width{ 0 };
		int a_height{ 0 };
	};
}