#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class ScreenShake
{
public:
	void Start(sf::Time duration, float intensity);
	sf::Vector2f Update(sf::Time dt);
	bool IsActive() const;
	
private:
	sf::Time m_time = sf::Time::Zero;
	float m_intensity = 0.f;
};