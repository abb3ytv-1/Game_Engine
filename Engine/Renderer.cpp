#include "pch.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Transform.h"
#include "Model.h"
#include "MathUtils.h"
#include "Texture.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <iostream>

using namespace nu;

// Initialize
bool Renderer::Initialize( const char* name, int width, int height ) {
	a_width = width;
	a_height = height;

	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr
			<< "SDL initialization failed: "
			<< SDL_GetError()
			<< '\n';

		return false;
	}

	if (!TTF_Init()) {
		std::cerr
			<< "SDL_ttf initialization failed: "
			<< SDL_GetError()
			<< '\n';

		SDL_Quit();
		return false;
	}

	a_window = SDL_CreateWindow( name, width, height, 0 );

	if (a_window == nullptr) {
		std::cerr
			<< "Window creation failed: "
			<< SDL_GetError()
			<< '\n';

		TTF_Quit();
		SDL_Quit();
		return false;
	}

	a_renderer = SDL_CreateRenderer( a_window, nullptr );

	if (a_renderer == nullptr) {
		std::cerr
			<< "Renderer creation failed: "
			<< SDL_GetError()
			<< '\n';

		SDL_DestroyWindow(a_window);
		a_window = nullptr;

		TTF_Quit();
		SDL_Quit();
		return false;
	}

	SDL_SetRenderVSync(a_renderer, 1);

	// Enable alpha blending for textures and particles
	SDL_SetRenderDrawBlendMode(a_renderer, SDL_BLENDMODE_BLEND);

	return true;
}

void Renderer::Shutdown() {
	SDL_DestroyRenderer(a_renderer);
	SDL_DestroyWindow(a_window);

	a_renderer = nullptr;
	a_window = nullptr;

	TTF_Quit();
	SDL_Quit();
}

// Clear
void Renderer::Clear() const
{
	SDL_RenderClear(a_renderer);
}

// Present
void Renderer::Present() const
{
	SDL_RenderPresent(a_renderer);
}

//Setcolor
void Renderer::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
{
	SDL_SetRenderDrawColor(a_renderer, r, g, b, a);
}

// Draw point
void Renderer::DrawPoint(float x, float y) const
{
	SDL_RenderPoint(a_renderer, x, y);
}

// Draw Filled Rectangle
void Renderer::DrawFillRect(float x, float y, float w, float h) const 
{
	SDL_FRect rect = { x, y, w, h };
	SDL_RenderFillRect(a_renderer, &rect);
}

// Draw Rectangle Outline
void Renderer::DrawRect(float x, float y, float w, float h) const
{
	SDL_FRect rect = { x, y, w, h };
	SDL_RenderRect(a_renderer, &rect);
}

// Draw Line
void Renderer::DrawLine(float x1, float y1, float x2, float y2) const
{
	SDL_RenderLine(a_renderer, x1, y1, x2, y2);
}



// Draw Model
void Renderer::DrawModel( const Model& model, const Transform& transform) const {
	for (const Mesh& mesh : model.GetMeshes()) {
		DrawMesh(mesh, transform);
	}
}

void Renderer::DrawTexture( const Texture& texture, const Transform& transform, float scale, const Vector2& origin, float alpha ) const {
	// Delegate to the vector-scale overload
	DrawTexture(texture, transform, Vector2{ scale, scale }, origin, alpha);
}

void Renderer::DrawTexture( const Texture& texture, const Transform& transform, const Vector2& scale, const Vector2& origin, float alpha ) const {
	if (texture.a_texture == nullptr) {
		return;
	}

	Vector2 textureSize = texture.GetSize();

	float width = textureSize.x * scale.x;
	float height = textureSize.y * scale.y;

	SDL_FRect destinationRect{
		transform.position.x - (origin.x * width),
		transform.position.y - (origin.y * height),
		width,
		height
	};

	// Apply texture alpha modulation for fading (alpha in 0.0 - 1.0)
	Uint8 alphaByte = static_cast<Uint8>(std::clamp(alpha, 0.0f, 1.0f) * 255.0f);

	// Temporarily set texture alpha modulation
	SDL_SetTextureAlphaMod(texture.a_texture, alphaByte);

	if (!SDL_RenderTextureRotated( a_renderer, texture.a_texture, nullptr, &destinationRect, transform.rotation, nullptr, SDL_FLIP_NONE )) {
		std::cerr
			<< "Could not draw texture: "
			<< SDL_GetError()
			<< '\n';
	}

	// Restore full alpha to avoid affecting other draws
	SDL_SetTextureAlphaMod(texture.a_texture, 255);
}

void Renderer::DrawMesh( const Mesh& mesh, const Transform& transform ) const { 
	const Color& color = mesh.GetColor();

	SetColor(
		static_cast<Uint8>(color.r * 255.0f),
		static_cast<Uint8>(color.g * 255.0f),
		static_cast<Uint8>(color.b * 255.0f),
		255
	);

	const auto& points = mesh.GetPoints();

	for (std::size_t i = 0; i + 1 < points.size(); i++) {
		Vector2 v1 = points[i];
		Vector2 v2 = points[i + 1];

		// Convert from model space to world space
		v1 *= transform.scale;
		v2 *= transform.scale;

		v1 = v1.Rotate(transform.rotation * DegToRad);
		v2 = v2.Rotate(transform.rotation * DegToRad);

		v1 += transform.position;
		v2 += transform.position;

		DrawLine(v1.x, v1.y, v2.x, v2.y);
	}
}

