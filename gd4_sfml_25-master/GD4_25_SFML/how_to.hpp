#pragma once
#include "state.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include "container.hpp"

class HowToState : public State
{
public:
	HowToState(StateStack& stack, Context context);
	virtual void Draw() override;
	virtual bool Update(sf::Time dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	sf::Text m_title_text;
	sf::Text m_howto_text;
	gui::Container m_gui_container;
};
