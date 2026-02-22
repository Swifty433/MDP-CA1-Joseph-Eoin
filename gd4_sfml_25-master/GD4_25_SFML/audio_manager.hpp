// Audio code was done by Joseph Byrne, with some help from SFML documentation. It handles Music and Sound Effects, as well as muting audio.
// The use of .wav files for sound assets and .flac files for larger music files!
#pragma once
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <map>
#include <list>

enum class Music
{
	//two music tracks, one for menu and one for game.
	kMenuMusic,
	kLevelMusic
};

enum class SoundEffects
{
	//sound effects for the game.
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
	//method to load the sound effects.
	void load_sound(SoundEffects sound, const std::string& filename);

private:
	// music, streaming music is better for larger sized files as opposed to loading the files into the sound buffer.
	sf::Music m_background_music;
	std::map<Music, std::string> m_music_filenames;
	float m_music_volume;

	// sound buffer used to load sounds from file and store for later
	std::map<SoundEffects, sf::SoundBuffer> m_sound_buffers;
	std::list<sf::Sound> m_sounds;
	float m_sound_volume;

	//bools for the music and sound toggles.
	bool m_music_on;
	bool m_sound_on;
};

