#include "container.hpp"

gui::Container::Container() : m_selected_child(-1)
{
}

void gui::Container::Pack(Component::Ptr component)
{
    m_children.emplace_back(component);
    if (!HasSelection() && component->IsSelectable())
    {
        Select(m_children.size() - 1);
    }
}

bool gui::Container::IsSelectable() const
{
    return false;
}

void gui::Container::HandleEvent(const sf::Event& event)
{
    const auto* key_released = event.getIf<sf::Event::KeyReleased>();
    if (HasSelection() && m_children[m_selected_child]->IsActive())
    {
        m_children[m_selected_child]->HandleEvent(event);
    }  
    else if(key_released)
    {
        if (key_released->scancode == sf::Keyboard::Scancode::W || key_released->scancode == sf::Keyboard::Scancode::Up)
        {
            SelectPrevious();
        }
        else if (key_released->scancode == sf::Keyboard::Scancode::S || key_released->scancode == sf::Keyboard::Scancode::Down)
        {
            SelectNext();
        }
        else if (key_released->scancode == sf::Keyboard::Scancode::Enter || key_released->scancode == sf::Keyboard::Scancode::Space)
        {
            if (HasSelection())
            {
                m_children[m_selected_child]->Activate();
            }
        }
    }

    const auto* mouse_moved = event.getIf<sf::Event::MouseMoved>();
    if (mouse_moved)
    {
        sf::Vector2f mouse_pos(mouse_moved->position.x, mouse_moved->position.y);

        for (std::size_t i = 0; i < m_children.size(); ++i)
        {
            if (m_children[i]->IsSelectable())
            {
                sf::FloatRect bounds = m_children[i]->getTransform().transformRect(
                    m_children[i]->getLocalBounds());

                if (bounds.contains(mouse_pos))
                {
                    Select(i);
                    break;
                }
            }
        }
    }

	const auto* mouse_button_released = event.getIf<sf::Event::MouseButtonReleased>();
    if (mouse_button_released && mouse_button_released->button == sf::Mouse::Button::Left)
    {
        sf::Vector2f mouse_pos(mouse_button_released->position.x, mouse_button_released->position.y);

        for (std::size_t i = 0; i < m_children.size(); ++i)
        {
            if (m_children[i]->IsSelectable())
            {
                sf::FloatRect bounds = m_children[i]->getTransform().transformRect
                    (m_children[i]->getLocalBounds());
                if (bounds.contains(mouse_pos))
                {
                    Select(i);
                    m_children[i]->Activate();
                    break;
                }
            }
		}
    }
}

sf::FloatRect gui::Container::getLocalBounds() const
{
    return sf::FloatRect();
}

void gui::Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const Component::Ptr& child : m_children)
    {
        target.draw(*child, states);
    }
}

bool gui::Container::HasSelection() const
{
    return m_selected_child >= 0;
}

void gui::Container::Select(std::size_t index)
{
    if (index < m_children.size() && m_children[index]->IsSelectable())
    {
        if (HasSelection())
        {
            m_children[m_selected_child]->Deselect();
        }
        m_children[index]->Select();
        m_selected_child = index;
    }
}

void gui::Container::SelectNext()
{
    if (!HasSelection())
    {
        return;
    }
    //Search for next selectable component
    int next = m_selected_child;
    do
    {
        next = (next + 1) % m_children.size();
    } while (!m_children[next]->IsSelectable());
    Select(next);
}

void gui::Container::SelectPrevious()
{
    if (!HasSelection())
    {
        return;
    }
    //Search for next selectable component
    int prev = m_selected_child;
    do
    {
        prev = (prev + m_children.size() - 1) % m_children.size();
    } while (!m_children[prev]->IsSelectable());
    Select(prev);
}
