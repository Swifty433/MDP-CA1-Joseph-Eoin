// Audio code was done by Joseph Byrne, with some help from SFML documentation. It handles Music and Sound Effects, as well as muting audio.
// The use of .wav files for sound assets and .flac files for larger music files!
#include "audio_manager.hpp"
#include<SFML/Audio/Listener.hpp>
#include <cmath>
#include <iostream>

namespace
{
	//volume constants
	const float max_volume = 100.0f;
	const float min_volume = 0.0f;

	// didnt add volume slider and opted for a mute button instead, but are here in future.
	const float volume_step = 5.0f;

	//defaults volumes for the music and sound effects
	const float default_music_volume = 50.0f;
	const float default_sound_volume = 70.0f;
}

//contructer for the audio manager
Audio_Manager::Audio_Manager()
	: m_background_music()
	, m_music_filenames()
	, m_music_volume(default_music_volume)
	//sound buffer used to load the sound effect files and store them for use when playing the effects in game.
	, m_sound_buffers()
	, m_sounds()
	, m_sound_volume(default_sound_volume)
	//boolean to check if the sound is playing or not, this is used for the sound toggle funtion in order to mute the sound and music in game.
	, m_music_on(true)
	, m_sound_on(true)
{
	//loading of the music and sound effect filenames
	//streaming music, better for larger sized files such as music as it doesnt load the whole file into memory at once! 
	m_music_filenames[Music::kMenuMusic] = "Media/Audio/MenuMusic.flac";
	m_music_filenames[Music::kLevelMusic] = "Media/Audio/LevelMusic.flac";

	//loading of the sound effects due to the fact they are smaller files which wont impact memory as much.
	load_sound(SoundEffects::kShoot, "Media/Audio/Shoot.wav");
	load_sound(SoundEffects::kButtonClick, "Media/Audio/ButtonClick.wav");
	load_sound(SoundEffects::kExplosion, "Media/Audio/Explosion.wav");
	load_sound(SoundEffects::kCollision, "Media/Audio/Collision.wav");

}

//music methods
// method to play the music 
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

	//couldnt get the music to loop properly, so i just left it to play once, but the music is quite long so it should be fine.
	//m_background_music.setLoop(true);
	m_background_music.play();
}

// method to stop the music
void Audio_Manager::stop_music()
{
	m_background_music.stop();
}

// method to pause the music
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

//method to play the sound effects in game, checks if the sound is loaded and if it is then it will play the sound effect, if not will out put a message, 
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

//method to stop all the sound effects from happening in game, which is used for the sound toggle function.
void Audio_Manager::stop_all_sounds()
{
	for (auto& sound : m_sounds)
	{
		sound.stop();
	}
	m_sounds.clear();
}

//volume toggle methods
// ***(ChatGPT was used to help with the implementation of the toggle functions)***
//opted for a toggle button instead of a volume slider, so this method will just toggle on and off the music and sound in the game. 
void Audio_Manager::toggle_music()
{
	m_music_on = !m_music_on;
	if (m_music_on)
	{
		m_background_music.play();
	}
	else
		//opted for pausing the music instead of stopping it as it will let the player resume the music from where they left of rather than restarting the music from the beginnin.
		m_background_music.pause();
}

//sound effect toggle. stops all sounds effects from playing when toggled off. 
void Audio_Manager::toggle_sound()
{
	m_sound_on = !m_sound_on;
	if (!m_sound_on)
	{
		stop_all_sounds();
	}
}

//used for toggle funtctions, checks if the music is on and same for sound effects. 
bool Audio_Manager::is_music_on() const { return m_music_on; }
bool Audio_Manager::is_sound_on()   const { return m_sound_on; }

//update function, removes any sound effect that has finished playing from the buffer.
void Audio_Manager::update()
{
	m_sounds.remove_if([](const sf::Sound& sound)
		{
			return sound.getStatus() == sf::Sound::Status::Stopped;
		});
}