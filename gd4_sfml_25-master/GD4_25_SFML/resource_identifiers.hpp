//Eoin Hamill D00258444
//Joseph Bryne D00255161

//edited by joseph byrne
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
	//added the shader declaration to manage shaders same way font and textures are.
	class Shader;
}

//template<typename Identifier, typename Resource>

typedef ResourceHolder<TextureID, sf::Texture> TextureHolder;
typedef ResourceHolder<FontID, sf::Font> FontHolder;
//shader holder added to manage shaders in the way textures and fonts are managed
typedef ResourceHolder<ShaderTypes, sf::Shader>  ShaderHolder;