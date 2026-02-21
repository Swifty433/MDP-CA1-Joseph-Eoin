#include "screenshake.hpp"
#include <random>

namespace
{
	float RandomFloat(float min, float max)
	{
		//random number generator
		static std::mt19937 gen(std::random_device{}());
		std::uniform_real_distribution<float> dis(min, max);
		return dis(gen);
	}
}

void ScreenShake::Start(sf::Time duration, float intensity)
{
	m_time = duration;
	m_intensity = intensity;
}

sf::Vector2f ScreenShake::Update(sf::Time dt)
{
	if(m_time <= sf::Time::Zero)
	{
		return sf::Vector2f(0.f, 0.f);
	}

	//Countdown
	m_time -= dt;
	if (m_time < sf::Time::Zero)
	{
		return { 0.f, 0.f };
	}

	//Generate random offset
	float offsetX = RandomFloat(-m_intensity, m_intensity);
	return { offsetX, 0.f };
}

bool ScreenShake::IsActive() const
{
	return m_time > sf::Time::Zero;
}