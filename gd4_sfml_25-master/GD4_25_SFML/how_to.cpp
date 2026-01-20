#include "how_to.hpp"
#include "utility.hpp"
#include "button.hpp"

HowToState::HowToState(StateStack& stack, Context context)
	: State(stack, context)
	, m_background_sprite(context.textures->Get(TextureID::kTitleScreen))
	, m_title_text(context.fonts->Get(FontID::kMain))
	, m_howto_text(context.fonts->Get(FontID::kMain))
{
	m_title_text.setString("How To Play");
	m_title_text.setCharacterSize(70);
	Utility::CentreOrigin(m_title_text);
	m_title_text.setPosition(sf::Vector2f(512.f, 200.f));

	std::string info = "HOW TO PLAY!";
	info += "\n\n- Use the arrow keys to move your character.";

	auto back_button = std::make_shared<gui::Button>(*context.fonts, *context.textures);
	back_button->setPosition(sf::Vector2f(500.f, 500.f));
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