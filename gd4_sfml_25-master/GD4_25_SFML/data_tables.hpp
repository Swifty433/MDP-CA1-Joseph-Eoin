//edited by Joseph Byrne
#pragma once
#include "texture_id.hpp"
#include "particletype.hpp"
#include <SFML/System/Time.hpp>
#include <functional>
#include "aircraft.hpp"

struct Direction
{
	Direction(float angle, float distance)
		: m_angle(angle), m_distance(distance) {
	}
	float m_angle;
	float m_distance;
};

struct AircraftData
{
	int m_hitpoints;
	float m_speed;
	TextureID m_texture;
	sf::Time m_fire_interval;
	//added the texture rect for the roll animation of the player 1 ship.
	sf::IntRect m_texture_rect;
	std::vector<Direction> m_directions;
	//added the bool for the roll animation of the player 1 ship.
	bool m_has_roll_animation;
};

struct ProjectileData
{
	int m_damage;
	float m_speed;
	TextureID m_texture;
	sf::IntRect m_texture_rect;
};

struct PickupData
{
	std::function<void(Aircraft&)> m_action;
	TextureID m_texture;
	sf::IntRect m_texture_rect;
};

//johns partical data but couldnt get working within our game.
struct ParticleData
{
	sf::Color m_color;
	sf::Time m_lifetime;
};

std::vector<AircraftData> InitializeAircraftData();
std::vector<ProjectileData> InitializeProjectileData();
std::vector<PickupData> InitializePickupData();
//initialiser for the particle data.
std::vector<ParticleData> InitializeParticleData();





