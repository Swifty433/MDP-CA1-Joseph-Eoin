//Eoin Hamill D00258444
//Joseph Bryne D00255161

//edited by Joseph Byrne
#pragma once
#include "component.hpp"
namespace gui
{
	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
		Container();
		void Pack(Component::Ptr component);
		virtual bool IsSelectable() const override;
		virtual void HandleEvent(const sf::Event& event) override;

		// setting up the constructor for the getLocalBounds function.
		virtual sf::FloatRect getLocalBounds() const override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		bool HasSelection() const;
		void Select(std::size_t index);
		void SelectNext();
		void SelectPrevious();

	private:
		std::vector<Component::Ptr> m_children;
		int m_selected_child;
	};
}

