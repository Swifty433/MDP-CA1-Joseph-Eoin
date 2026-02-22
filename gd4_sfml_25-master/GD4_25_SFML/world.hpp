//Eoin Hamill D00258444
//Joseph Bryne D00255161

//edited by joseph byrne
#pragma once
#include <SFML/Graphics.hpp>
#include "resource_identifiers.hpp"
#include "scene_node.hpp"
#include "scene_layers.hpp"
#include "aircraft.hpp"
#include "command_queue.hpp"
#include "bloom_effect.hpp"
#include "audio_manager.hpp"
#include "screenshake.hpp"

class SpriteNode;

class World
{
public:
	explicit World(sf::RenderTarget& output_target, FontHolder& font, Audio_Manager& audio);
	void Update(sf::Time dt);
	void Draw();

	CommandQueue& GetCommandQueue();

	//redundant
	//bool HasAlivePlayer() const;
	//bool HasPlayerReachedEnd() const;

	//new functions to check if the game has ended and to get the winner of the game
	bool HasGameEnded() const;
	int GetWinner() const;

private:
	void LoadTextures();
	void BuildScene();
	void AdaptPlayerVelocity();
	void AdaptPlayerPosition();

	void SpawnEnemies();
	void AddEnemies();
	void AddEnemy(AircraftType type, float relx, float rely);

	sf::FloatRect GetViewBounds() const;
	sf::FloatRect GetBattleFieldBounds() const;

	void GuideMissiles();

	void HandleCollisions();

	void DestroyEntitiesOutsideView();

private:
	struct SpawnPoint
	{
		SpawnPoint(AircraftType type, float x, float y) :m_type(type), m_x(x), m_y(y)
		{

		}
		AircraftType m_type;
		float m_x;
		float m_y;
	};

private:
	ScreenShake m_shake;
	sf::Vector2f m_camera_center;
	sf::RenderTarget& m_target;
	sf::RenderTexture m_scene_texture;

	sf::View m_camera;
	TextureHolder m_textures;
	FontHolder& m_fonts;
	SceneNode m_scene_graph;
	std::array<SceneNode*, static_cast<int>(SceneLayers::kLayerCount)> m_scene_layers;
	sf::FloatRect m_world_bounds;
	sf::Vector2f m_spawn_position;
	sf::Vector2f m_spawn_position_2;
	float m_scroll_speed;
	Aircraft* m_player_aircraft;
	Aircraft* m_player_aircraft_2;

	CommandQueue m_command_queue;


	std::vector<SpawnPoint> m_enemy_spawn_points;
	std::vector<Aircraft*> m_active_enemies;

	//johns new bloom effect for post processing
	BloomEffect m_bloom_effect;

	//variable to track the scrolling of the background.
	float m_background_scroll;

	SpriteNode* m_background_node;
	//added the audio manager to the class to be able to use it for aircraft and music and sounds.
	Audio_Manager* m_audio;
};

