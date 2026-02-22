//Eoin Hamill D00258444
//Joseph Bryne D00255161

//edited by Joseph Byrne
#include "label.hpp"
#include "button.hpp"

gui::Label::Label(const std::string& text, const FontHolder& font) : m_text(font.Get(FontID::kMain), text, 16)
{
	//changes the colour of the text to black.
    m_text.setFillColor(sf::Color::Black);
}

bool gui::Label::IsSelectable() const
{
    return false;
}


void gui::Label::SetText(const std::string& text)
{
    m_text.setString(text);
}


void gui::Label::HandleEvent(const sf::Event& event)
{
}

// returning the bound of the label.
sf::FloatRect gui::Label::getLocalBounds() const
{
    return m_text.getLocalBounds();
}

void gui::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_text, states);
}
