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
	, m_music_on(true)
	, m_sound_on(true)
{
	//loading of the music and sound effect filenames
	m_music_filenames[Music::kMenuMusic] = "Media/Audio/MenuMusic.flac";
	m_music_filenames[Music::kLevelMusic] = "Media/Audio/LevelMusic.flac";

	load_sound(SoundEffects::kShoot, "Media/Audio/Shoot.wav");
	load_sound(SoundEffects::kButtonClick, "Media/Audio/ButtonClick.wav");

}

//music methods
void Audio_Manager::play_music(Music music)
{
	if (!m_music_on) return;
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

void Audio_Manager::pause_music()
{
	m_background_music.pause();
}

//sound effect methods
void Audio_Manager::load_sound(SoundEffects sound, const std::string& filename)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(filename))
	{
		std::cout << "Cant load sound: " << filename << std::endl;
		return;
	}
	m_sound_buffers[sound] = buffer;
}

void Audio_Manager::play_sound(SoundEffects sound)
{
	if (!m_sound_on) return;
	auto buffer = m_sound_buffers.find(sound);
	if (buffer == m_sound_buffers.end())
	{
		std::cout << "sound not loaded" << std::endl;
		return;
	}

	//sound list
	m_sounds.emplace_back(buffer->second);
	sf::Sound& new_sound = m_sounds.back();
	new_sound.setVolume(m_sound_volume);
	new_sound.play();
}

void Audio_Manager::stop_all_sounds()
{
	for (auto& sound : m_sounds)
	{
		sound.stop();
	}
	m_sounds.clear();
}

//volume toggle methods
void Audio_Manager::toggle_music()
{
	m_music_on = !m_music_on;
	if (m_music_on)
	{
		m_background_music.play();
	}
	else
		m_background_music.pause();
}

void Audio_Manager::toggle_sound()
{
	m_sound_on = !m_sound_on;
	if (!m_sound_on)
	{
		stop_all_sounds();
	}
}

bool Audio_Manager::is_music_on() const { return m_music_on; }
bool Audio_Manager::is_sound_on()   const { return m_sound_on; }

void Audio_Manager::update()
{
	m_sounds.remove_if([](const sf::Sound& sound)
		{
			return sound.getStatus() == sf::Sound::Status::Stopped;
		});
}