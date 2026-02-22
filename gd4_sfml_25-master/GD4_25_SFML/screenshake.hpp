//Eoin Hamill D00258444
//Joseph Bryne D00255161

//Screenshake code made by Eoin Hamill With the help of this video ( https://www.youtube.com/shorts/emNyhHW7Bto) which helped me understand how it would be done by editing the camera and copilot 
#pragma once
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>


//Screnshake will edit the camera postion.
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