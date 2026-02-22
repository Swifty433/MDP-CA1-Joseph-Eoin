//Eoin Hamill D00258444
//Joseph Bryne D00255161

//edited by Joseph Byrne
#pragma once
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "player.hpp"
#include "resource_holder.hpp"
#include "resource_identifiers.hpp"
#include "statestack.hpp"
#include "audio_manager.hpp"

class Application
{
public:
	Application();
	void Run();

private:
	void ProcessInput();
	void Update(sf::Time dt);
	void Render();
	void RegisterStates();

private:
	sf::RenderWindow m_window;
	Player m_player=Player(1);
	Player m_player_2=Player(2);

	TextureHolder m_textures;
	FontHolder m_fonts;
	//added audio manager to the application class.
	Audio_Manager m_audio;

	StateStack m_stack;
};

