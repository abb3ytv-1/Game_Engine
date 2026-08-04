#pragma once
#include "Texture.h"
#include "Renderer.h"  
#include <SDL3/SDL.h>       
#include <SDL3_image/SDL_image.h>  

using namespace nu;

Texture::~Texture()
{
    // if texture exists, destroy texture
    if (a_texture) SDL_DestroyTexture(a_texture);
}

bool Texture::Load(const std::string& filename, Renderer& renderer)
{
    // load image onto surface
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface)
    {
        std::cerr << "Could not load image: " << filename << std::endl;
        return false;
    }


    a_texture = SDL_CreateTextureFromSurface(renderer.a_renderer, surface);
    SDL_DestroySurface(surface);
    if (!a_texture)
    {
        std::cerr << "Could not create texture: " << filename << std::endl;
        return false;
    }

    return true;
}

Vector2 Texture::GetSize()
{
    Vector2 v;
    // https://wiki.libsdl.org/SDL3/SDL_GetTextureSize
    SDL_GetTextureSize(a_texture, &v.x, &v.y);
    return v;
}