#include "audio_manager.hpp"
#include<SFML/Audio/Listener.hpp>
#include <cmath>
#include <iostream>

namespace
{
	const float max_volume = 100.0f;
	const float min_volume = 0.0f;
	const float volume_step = 5.0f;
	const float default_music_volume = 50.0f;

	const float default_sound_volume = 70.0f;
}

Audio_Manager::Audio_Manager()
	: m_background_music()
	, m_music_filenames()
	, m_music_volume(default_music_volume)
	, m_sound_buffers()
	, m_sounds()
	, m_sound_volume(default_sound_volume)
{
	//loading of the music and sound effect filenames
	m_music_filenames[Music::kMenuMusic] = "Media/Audio/MenuMusic.flac";
}

void Audio_Manager::play_music(Music music)
{
	std::string filename = m_music_filenames[music];
	if (!m_background_music.openFromFile(filename))
	{
		std::cout << "Cant load menu music" << filename << std::endl;
		return;
	}

	m_background_music.setVolume(m_music_volume);
	//m_background_music.setLoop(true);
	m_background_music.play();
}

void Audio_Manager::stop_music()
{
	m_background_music.stop();
}

void Audio_Manager::play_sound(SoundEffects sound)
{

}

void Audio_Manager::stop_all_sounds()
{
	
}

void Audio_Manager::update()
{

}