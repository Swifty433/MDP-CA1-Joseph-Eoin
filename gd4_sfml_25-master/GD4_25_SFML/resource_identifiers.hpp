#pragma once
#include "texture_id.hpp"
#include "resource_holder.hpp"
#include "shader_types.hpp"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Shader.hpp>
#include "fontID.hpp"

namespace sf
{
	class Texture;
	class Font;
	class Shader;
}

//template<typename Identifier, typename Resource>

typedef ResourceHolder<TextureID, sf::Texture> TextureHolder;
typedef ResourceHolder<FontID, sf::Font> FontHolder;
typedef ResourceHolder<ShaderTypes, sf::Shader>  ShaderHolder;