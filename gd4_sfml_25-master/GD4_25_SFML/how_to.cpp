#include "how_to.hpp"
#include "utility.hpp"
#include "button.hpp"

HowToState::HowToState(StateStack& stack, Context context)
	: State(stack, context)
	, m_background_sprite(context.textures->Get(TextureID::kAltBackground))
	, m_title_text(context.fonts->Get(FontID::kMain))
	, m_howto_text(context.fonts->Get(FontID::kMain))
{
	m_title_text.setString("How To Play");
	m_title_text.setCharacterSize(70);
	m_title_text.setFillColor(sf::Color::Black);
	Utility::CentreOrigin(m_title_text);
	m_title_text.setPosition(sf::Vector2f(512.f, 100.f));

	std::string info = "HOW TO PLAY!";
	info += "\n\n- Use the arrow keys to move Player 1.";
	info += "\n\n- Use the W & D to move Player 2";
	info += "\n\n- Player 1 can shoot at Player 2 ships";
	info += "\n\n- Player 2 can spawn ships to attack Player 1";
	info += "\n\n- Check out SETTINGS to change Player controls";
	m_howto_text.setFillColor(sf::Color::Black);
	m_howto_text.setString(info);
	m_howto_text.setCharacterSize(20);
	m_howto_text.setPosition(sf::Vector2f(300.f, 200.f));

	auto back_button = std::make_shared<gui::Button>(*context.fonts, *context.textures, *context.audio);
	back_button->setPosition(sf::Vector2f(80.f, 575.f));
	back_button->SetCallback(std::bind(&HowToState::RequestStackPop, this));
	back_button->SetText("Back to Menu");
	m_gui_container.Pack(back_button);
	//Utility::CentreOrigin(*back_button);
}

void HowToState::Draw()
{
	sf::RenderWindow& window = *GetContext().window;
	window.setView(window.getDefaultView());
	window.draw(m_background_sprite);
	window.draw(m_title_text);
	window.draw(m_howto_text);
	window.draw(m_gui_container);
}

bool HowToState::Update(sf::Time dt)
{
	return true;
}

bool HowToState::HandleEvent(const sf::Event& event)
{
	m_gui_container.HandleEvent(event);
	return false;
}