//Eoin Hamill D00258444
//Joseph Bryne D00255161

//Eoin set up player 2s ship + the resource meter and how it interacts with spawning enemies.

//edited by Joseph Byrne
#include "aircraft.hpp"
#include "texture_id.hpp"
#include "data_tables.hpp"
#include "utility.hpp"
#include "constants.hpp"
#include "pickup_type.hpp"
#include "projectile.hpp"
#include "pickup.hpp"
#include "projectile_type.hpp"
#include <iostream>


namespace
{
	const std::vector<AircraftData> Table = InitializeAircraftData();
}

TextureID ToTextureID(AircraftType type)
{
	switch (type)
	{
	case AircraftType::kEagle:
		return TextureID::kEagle;
		break;
	case AircraftType::kRaptor:
		return TextureID::kRaptor;
		break;
		//added the player 2 ship type.
	case AircraftType::kPlayer2Ship:
		return TextureID::kPlayer2Ship;
		break;
	}
	return TextureID::kEagle;
}

Aircraft::Aircraft(AircraftType type, const TextureHolder& textures, const FontHolder& fonts, Audio_Manager& audio) //audio manager added to constructor
	: Entity(Table[static_cast<int>(type)].m_hitpoints) 
	, m_type(type) 
	, m_sprite(textures.Get(Table[static_cast<int>(type)].m_texture))
	, m_health_display(nullptr)
	, m_missile_display(nullptr)
	, m_resource_display(nullptr)
	, m_distance_travelled(0.f) 
	, m_directions_index(0)
	, m_fire_rate(1)
	, m_spread_level(1)
	, m_is_firing(false)
	, m_is_launching_missile(false)
	, m_fire_countdown(sf::Time::Zero)
	, m_missile_ammo(2)
	, m_is_marked_for_removal(false)
	, m_spawned_pickup(false)
	//audio manager refrence
	, m_audio(&audio)
	//johns explosion animation code
	, m_show_explosion(true)
	,m_explosion(textures.Get(TextureID::kExplosion))
{
	//texture rect for the roll animation of the player ship.
	if (Table[static_cast<int>(type)].m_has_roll_animation)
	{
		m_sprite.setTextureRect(Table[static_cast<int>(type)].m_texture_rect);
	}
	Utility::CentreOrigin(m_sprite);
	m_sprite.setScale(
		sf::Vector2f(Table[static_cast<int>(type)].m_scale,
			Table[static_cast<int>(type)].m_scale)
	);
		

	//SpawnEnemyCommand
	//Resource meter set up.
	m_resource_meter = 0.f;
	m_spawn_enemy = false;

	//Explosion animation set up
	m_explosion.SetFrameSize(sf::Vector2i(256, 256));
	m_explosion.SetNumFrames(16);
	m_explosion.SetDuration(sf::seconds(1));
	Utility::CentreOrigin(m_explosion);

	m_spawn_enemy_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_spawn_enemy_command.action = [this, &textures, &fonts](SceneNode& node, sf::Time)
		{
			sf::Vector2f p = this->GetWorldPosition();

			std::unique_ptr<Aircraft> enemy(new Aircraft(m_pending_spawn_type, textures, fonts, *m_audio));
			enemy->setPosition(sf::Vector2f(p.x, p.y + 60.f));

			node.AttachChild(std::move(enemy));
		};

	//Fire bullet command
	m_fire_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_fire_command.action = [this, &textures](SceneNode& node, sf::Time dt)
		{		
			CreateBullet(node, textures);
		};

	m_missile_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_missile_command.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			CreateProjectile(node, ProjectileType::kMissile, 0.f, 0.5f, textures);
		};
	m_drop_pickup_command.category = static_cast<int>(ReceiverCategories::kScene);
	m_drop_pickup_command.action = [this, &textures](SceneNode& node, sf::Time dt)
		{
			CreatePickup(node, textures);
		};

	std::string* health = new std::string("");
	std::unique_ptr<TextNode> health_display(new TextNode(fonts, *health));
	m_health_display = health_display.get();
	AttachChild(std::move(health_display));

	//If its player 2 ship we add the resource meter.
	if (type == AircraftType::kPlayer2Ship)
	{
		std::string* resource = new std::string("");
		std::unique_ptr<TextNode> resource_display(new TextNode(fonts, *resource));
		m_resource_display = resource_display.get();
		AttachChild(std::move(resource_display));
	}

	if (Aircraft::GetCategory() == static_cast<int>(ReceiverCategories::kPlayerAircraft))
	{
		std::string* missile_ammo = new std::string("");
		std::unique_ptr<TextNode> missile_display(new TextNode(fonts, *missile_ammo));
		m_missile_display = missile_display.get();
		AttachChild(std::move(missile_display));
	}
	UpdateTexts();

	

}

unsigned int Aircraft::GetCategory() const
{
	if (IsAllied())
	{
		//if its player 1 aircraft send back that it's a player aircraft and it's player 1
		if (m_player_id == 1)
		{
			return static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft)
				| static_cast<unsigned int>(ReceiverCategories::kPlayer1Aircraft) ;
		}
		if (m_player_id == 2)
		{
			//not sending back player aircraft here as we don't want it to collide with the spawned enemies
			return static_cast<unsigned int>(ReceiverCategories::kPlayer2Aircraft);
				//| static_cast<unsigned int>(ReceiverCategories::kPlayer2Aircraft);
		}
		return static_cast<unsigned int>(ReceiverCategories::kAlliedAircraft);
	}
	return static_cast<unsigned int>(ReceiverCategories::kEnemyAircraft);
}

void Aircraft::IncreaseFireRate()
{
	if (m_fire_rate < kMaxFireRate)
	{
		++m_fire_rate;
	}
}

void Aircraft::IncreaseFireSpread()
{
	if (m_spread_level < kMaxSpread)
	{
		++m_spread_level;
	}
}

void Aircraft::CollectMissile(unsigned int count)
{
	m_missile_ammo += count;
}

void Aircraft::UpdateTexts()
{
	m_health_display->SetString(std::to_string(GetHitPoints()) + "HP");
	m_health_display->setPosition(sf::Vector2f(0.f, 50.f));
	m_health_display->setRotation(-getRotation());

	if (m_missile_display)
	{
		m_missile_display->setPosition(sf::Vector2f(0.f, 70.f));
		if (m_missile_ammo == 0)
		{
			m_missile_display->SetString("");
		}
		else
		{
			m_missile_display->SetString("M: " + std::to_string(m_missile_ammo));
		}
	}

	if (m_resource_display)
	{
		m_resource_display->setPosition(sf::Vector2f(0.f, 80.f));
		m_resource_display->setRotation(-getRotation());

		int res = static_cast<int>(m_resource_meter);
		m_resource_display->SetString(std::to_string(res) + "/" + std::to_string((int)m_resource_meter_max));
	}
}

void Aircraft::UpdateMovementPattern(sf::Time dt)
{
	//Enemy AI
	const std::vector<Direction>& directions = Table[static_cast<int>(m_type)].m_directions;
	if (!directions.empty())
	{
		//Move along the current direction for distance and then change direction
		if (m_distance_travelled > directions[m_directions_index].m_distance)
		{
			m_directions_index = (m_directions_index + 1) % directions.size();
			m_distance_travelled = 0;
		}

		//Compute the velocity
		//Add 90 to move down the screen, 0 degrees is to the right
		double radians = Utility::toRadians(directions[m_directions_index].m_angle + 90.f);
		float vx = GetMaxSpeed() * std::cos(radians);
		float vy = GetMaxSpeed() * std::sin(radians);

		SetVelocity(sf::Vector2f(vx, vy));
		m_distance_travelled += GetMaxSpeed() * dt.asSeconds();
	}
}

float Aircraft::GetMaxSpeed() const
{
	return Table[static_cast<int>(m_type)].m_speed;
}

void Aircraft::Fire()
{
	if (Table[static_cast<int>(m_type)].m_fire_interval != sf::Time::Zero)
	{
		m_is_firing = true;
	}
}

void Aircraft::LaunchMissile()
{
	if (m_missile_ammo > 0)
	{
		m_is_launching_missile = true;
		--m_missile_ammo;
	}
}

void Aircraft::CreateBullet(SceneNode& node, const TextureHolder& textures) const
{
	ProjectileType type = IsAllied() ? ProjectileType::kAlliedBullet : ProjectileType::kEnemyBullet;
	switch (m_spread_level)
	{
	case 1:
		CreateProjectile(node, type, 0.0f, 0.5f, textures);
		break;
	case 2:
		CreateProjectile(node, type, -0.5f, 0.5f, textures);
		CreateProjectile(node, type, 0.5f, 0.5f, textures);
		break;
	case 3:
		CreateProjectile(node, type, 0.0f, 0.5f, textures);
		CreateProjectile(node, type, -0.5f, 0.5f, textures);
		CreateProjectile(node, type, 0.5f, 0.5f, textures);
		break;
	}
}

void Aircraft::CreateProjectile(SceneNode& node, ProjectileType type, float x_offset, float y_offset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
	sf::Vector2f offset(x_offset * m_sprite.getGlobalBounds().size.x, y_offset * m_sprite.getGlobalBounds().size.y);
	sf::Vector2f velocity(0, projectile->GetMaxSpeed());

	float sign = IsAllied() ? -1.f: 1.f;
	projectile->setPosition(GetWorldPosition() + offset * sign);
	projectile->SetVelocity(velocity * sign);
	node.AttachChild(std::move(projectile));

}

sf::FloatRect Aircraft::GetBoundingRect() const
{
	return GetWorldTransform().transformRect(m_sprite.getGlobalBounds());
}

bool Aircraft::IsMarkedForRemoval() const
{
	//if the aircraft is destroyed and the explosion animation has finished, mark it for removal
	if (IsDestroyed() && !m_is_marked_for_removal)
	{
		m_audio->play_sound(SoundEffects::kExplosion);
		const_cast<Aircraft*>(this)->m_is_marked_for_removal = true;
	}
	//only remove the aircraft once the animation has finished
	return IsDestroyed() && (m_explosion.IsFinished() || !m_show_explosion);
}

void Aircraft::DrawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	//draw explosion if destroyed, otherwise draw the aircraft sprite
	if (IsDestroyed() && m_show_explosion)
		target.draw(m_explosion, states);
	else
		target.draw(m_sprite, states);
}

void Aircraft::UpdateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (IsDestroyed())
	{
		CheckPickupDrop(commands);
		//m_explosion is updated after aircraft is destroyed, once the animation is finished the aircraft will be removed from the scene.
		m_explosion.Update(dt);
		return;
	}
	Entity::UpdateCurrent(dt, commands);
	UpdateTexts();
	UpdateMovementPattern(dt);

	//roll anim for player ship
	UpdateRollAnimation();

	//Check if bullets or missiles were fired
	CheckProjectileLaunch(dt, commands);
	CheckEnemySpawn(commands);

	UpdateResourceMeter(dt);
	TrySpawnEnemy(commands);
}

void Aircraft::CheckProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	if (!IsAllied())
	{
		Fire();
	}

	if (m_is_firing && m_fire_countdown <= sf::Time::Zero)
	{
		commands.Push(m_fire_command);
		m_audio->play_sound(SoundEffects::kShoot);
		m_fire_countdown += Table[static_cast<int>(m_type)].m_fire_interval / (m_fire_rate + 1.f);
		m_is_firing = false;
	}
	else if (m_fire_countdown > sf::Time::Zero)
	{
		m_fire_countdown -= dt;
		m_is_firing = false;
	}

	//Missile launch
	if (m_is_launching_missile)
	{
		commands.Push(m_missile_command);
		m_is_launching_missile = false;
	}
}

//checking if the aircraft is allied for player 1 and player 2 editted by eoin hamill
bool Aircraft::IsAllied() const
{
	return m_type == AircraftType::kEagle || m_type == AircraftType::kPlayer2Ship;
}

void Aircraft::CreatePickup(SceneNode& node, const TextureHolder& textures)
{
	if (!m_spawned_pickup)
	{
		auto type = static_cast<PickupType>(Utility::RandomInt(static_cast<int>(PickupType::kPickupCount)));
		std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
		pickup->setPosition(GetWorldPosition());
		//changed pickup velocity so the power ups move down the screen.
		pickup->SetVelocity(0.f, 100.f);
		node.AttachChild(std::move(pickup));
	}
	m_spawned_pickup = true;
}

void Aircraft::CheckPickupDrop(CommandQueue& commands)
{
	if (!IsAllied() && Utility::RandomInt(kPickupDropChance) == 0 && !m_spawned_pickup)
	{
		commands.Push(m_drop_pickup_command);
	}
}

// roll animation for the player ship depending if the player is moving right or left - this code is from johns github page
void Aircraft::UpdateRollAnimation()
{
	if (Table[static_cast<int>(m_type)].m_has_roll_animation)
	{
		sf::IntRect textureRect = Table[static_cast<int>(m_type)].m_texture_rect;

		//Roll left: Texture rect is offset once
		if (GetVelocity().x < 0.f)
		{
			textureRect.position.x += textureRect.size.x;
		}
		else if (GetVelocity().x > 0.f)
		{
			textureRect.position.x += 2 * textureRect.size.x;
		}
		m_sprite.setTextureRect(textureRect);

	}
}

void Aircraft::SetPlayerid(int id)
{
	m_player_id = id;
}

void Aircraft::SpawnEnemy()
{
	m_spawn_enemy = true;
}

void Aircraft::CheckEnemySpawn(CommandQueue& commands)
{
	if (m_spawn_enemy)
	{
		commands.Push(m_spawn_enemy_command);
		m_spawn_enemy = false;
	}
}

//Spawning Enemy with resource meter
//Request to spawn
void Aircraft::RequestSpawnEnemy(AircraftType type)
{
	m_pending_spawn_type = type;
	m_spawn_requested = true;
}

//Add to the resource meter
void Aircraft::UpdateResourceMeter(sf::Time dt)
{
	m_resource_meter += m_resource_regen * dt.asSeconds();
	if (m_resource_meter > m_resource_meter_max)
		m_resource_meter = m_resource_meter_max;
}

//Check if you got enough to spawn enemy.
void Aircraft::TrySpawnEnemy(CommandQueue& commands)
{
	const float cost = Table[static_cast<int>(m_pending_spawn_type)].m_spawn_cost;
	if (m_spawn_requested && m_resource_meter >= cost)
	{
		m_resource_meter -= cost;
		commands.Push(m_spawn_enemy_command);
	}

	m_spawn_requested=false;
}




