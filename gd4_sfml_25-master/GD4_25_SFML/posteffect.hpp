//Eoin Hamill D00258444
//Joseph Bryne D00255161

// post effect to apply a shader to a target - johns code from github
#pragma once

namespace sf
{
	class RenderTarget;
	class RenderTexture;
	class Shader;
}

class PostEffect
{
public:
	virtual ~PostEffect();
	virtual void Apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
	static bool IsSupported();

protected:
	static void ApplyShader(const sf::Shader& shader, sf::RenderTarget& output);

};

