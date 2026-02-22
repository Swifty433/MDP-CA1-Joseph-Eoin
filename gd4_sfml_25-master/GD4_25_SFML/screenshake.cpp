//Eoin Hamill D00258444
//Joseph Bryne D00255161


//Screenshake code made by Eoin Hamill With the help of this video ( https://www.youtube.com/shorts/emNyhHW7Bto) which helped me understand how it would be done by editing the camera and copilot 
#include "screenshake.hpp"
#include <random>

namespace
{
	float RandomFloat(float min, float max)
	{
		//random number generator
		static std::mt19937 gen(std::random_device{}());
		//set min max
		std::uniform_real_distribution<float> dis(min, max);
		//return random num
		return dis(gen);
	}
}

void ScreenShake::Start(sf::Time duration, float intensity)
{
	//how long shake lasts
	m_time = duration;
	//huch much camera moves
	m_intensity = intensity;
}

sf::Vector2f ScreenShake::Update(sf::Time dt)
{
	//if no time left stop shaking
	if(m_time <= sf::Time::Zero)
	{
		//set movement back to 0
		return sf::Vector2f(0.f, 0.f);
	}

	//Countdown
	m_time -= dt;
	//if time went to 0 stop shaking
	if (m_time < sf::Time::Zero)
	{
		return { 0.f, 0.f };
	}

	//Generate random offset moving left or right slightly horizontally
	float offsetX = RandomFloat(-m_intensity, m_intensity);
	return { offsetX, 0.f };
}
//checking if its still active
bool ScreenShake::IsActive() const
{
	return m_time > sf::Time::Zero;
}