#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>
#include <list>

enum class Music
{
	kMenuMusic,
	kLevelMusic
};

enum class SoundEffects
{
	kShoot,
	kButtonClick
};

class Audio_Manager
{
public:
	Audio_Manager();

	//functions for the music
	void play_music(Music music);
	void stop_music();

	//functions for the sound effects
	void play_sound(SoundEffects sound);
	void stop_all_sounds();

	//update function
	void update();

private:
	sf::Music m_background_music;
	std::map<Music, std::string> m_music_filenames;
	float m_music_volume;

	std::map<SoundEffects, sf::SoundBuffer> m_sound_buffers;
	std::list<sf::Sound> m_sounds;
	float m_sound_volume;

};

