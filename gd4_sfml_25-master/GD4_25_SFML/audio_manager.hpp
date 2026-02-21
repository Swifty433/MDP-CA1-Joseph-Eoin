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
	kButtonClick,
	kExplosion,
	kCollision
};

class Audio_Manager
{
public:
	Audio_Manager();

	//functions for the music
	void play_music(Music music);
	void stop_music();
	void pause_music();

	//functions for the sound effects
	void play_sound(SoundEffects sound);
	void stop_all_sounds();

	//volume toggle
	void toggle_music();
	void toggle_sound();
	bool is_music_on() const;
	bool is_sound_on() const;


	//update function
	void update();

private:
	void load_sound(SoundEffects sound, const std::string& filename);

private:
	sf::Music m_background_music;
	std::map<Music, std::string> m_music_filenames;
	float m_music_volume;

	std::map<SoundEffects, sf::SoundBuffer> m_sound_buffers;
	std::list<sf::Sound> m_sounds;
	float m_sound_volume;

	bool m_music_on;
	bool m_sound_on;
};

