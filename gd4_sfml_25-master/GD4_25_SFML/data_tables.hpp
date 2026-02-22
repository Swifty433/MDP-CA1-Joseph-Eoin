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
	sf::IntRect m_texture_rect;
	std::vector<Direction> m_directions;
	bool m_has_roll_animation;

	float m_scale = 1.f;
	float m_spawn_cost = 20.f;
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

struct ParticleData
{
	sf::Color m_color;
	sf::Time m_lifetime;
};

std::vector<AircraftData> InitializeAircraftData();
std::vector<ProjectileData> InitializeProjectileData();
std::vector<PickupData> InitializePickupData();
std::vector<ParticleData> InitializeParticleData();





