//Eoin Hamill D00258444
//Joseph Bryne D00255161

#pragma once
#include "scene_node.hpp"

class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);
	void SetTextureRect(sf::IntRect rect) { m_sprite.setTextureRect(rect); }

private:
	virtual void DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite m_sprite;
	
};

