#include "player.hpp"
#include "aircraft.hpp"

struct AircraftMover
{
    //Changed movement using acceleration instead of a set speed.
    // Only worrying about the x axis
    AircraftMover(float ax)
        : m_acceleration(ax)
    { }
    //based on how much time has passed speed up. 
    void operator()(Aircraft& aircraft, sf::Time dt) const
    {
        aircraft.Accelerate(sf::Vector2f(
            m_acceleration * dt.asSeconds(), 0.f
        ));
    }

    float m_acceleration;
};

Player::Player(int player_num)
{
    m_player_number = player_num;
    //PLAYER 1 Bottom Player
    if (m_player_number == 1)
    {
        m_key_binding[sf::Keyboard::Scancode::Left] = Action::kMoveLeft;
        m_key_binding[sf::Keyboard::Scancode::Right] = Action::kMoveRight;
        //No need for up and down movement so I commented it out for now.
        //m_key_binding[sf::Keyboard::Scancode::W] = Action::kMoveUp;
        //m_key_binding[sf::Keyboard::Scancode::S] = Action::kMoveDown;
        m_key_binding[sf::Keyboard::Scancode::Up] = Action::kBulletFire;
        m_key_binding[sf::Keyboard::Scancode::Down] = Action::kMissileFire;
    }
    //Player 2 controls top player spawns enemies
    else if(m_player_number == 2)
    {
        m_key_binding[sf::Keyboard::Scancode::A] = Action::kMoveLeftPlayer2;
        m_key_binding[sf::Keyboard::Scancode::D] = Action::kMoveRightPlayer2;
        m_key_binding[sf::Keyboard::Scancode::Q] = Action::kSpawnAlienPlayer2;
        m_key_binding[sf::Keyboard::Scancode::W] = Action::kSpawnAlien2Player2;
        m_key_binding[sf::Keyboard::Scancode::E] = Action::kSpawnAlien3Player2;
    }

    InitialiseActions();

    for (auto& pair : m_action_binding)
    {
        pair.second.category = static_cast<unsigned int>(ReceiverCategories::kPlayerAircraft);
    }
}

void Player::HandleEvent(const sf::Event& event, CommandQueue& command_queue)
{
    const auto* key_pressed = event.getIf<sf::Event::KeyPressed>();
    if (key_pressed)
    {
        auto found = m_key_binding.find(key_pressed->scancode);
        if (found != m_key_binding.end() && !IsRealTimeAction(found->second))
        {
            command_queue.Push(m_action_binding[found->second]);
        }
    }
}

void Player::HandleRealTimeInput(CommandQueue& command_queue)
{
    for (auto pair : m_key_binding)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && IsRealTimeAction(pair.second))
        {
            command_queue.Push(m_action_binding[pair.second]);
        }
    }
}

void Player::AssignKey(Action action, sf::Keyboard::Scancode key)
{
    //Remove keys that are currently bound to the action
    for (auto itr = m_key_binding.begin(); itr != m_key_binding.end();)
    {
        if (itr->second == action)
        {
            m_key_binding.erase(itr++);
        }
        else
        {
            ++itr;
        }
    }
    m_key_binding[key] = action;
}

sf::Keyboard::Scancode Player::GetAssignedKey(Action action) const
{
    for (auto pair : m_key_binding)
    {
        if (pair.second == action)
        {
            return pair.first;
        }
    }
    return sf::Keyboard::Scancode::Unknown;
}

void Player::SetMissionStatus(MissionStatus status)
{
    m_current_mission_status = status;
}

MissionStatus Player::GetMissionStatus() const
{
    return m_current_mission_status;
}

void Player::InitialiseActions()
{
    //players accel
    const float kPlayerAcceleration = 300.f;
    //Using negative since its backwards to accel left
    m_action_binding[Action::kMoveLeft].action = 
        DerivedAction<Aircraft>(AircraftMover(-kPlayerAcceleration));

    //Using positive to accel right
    m_action_binding[Action::kMoveRight].action =
        DerivedAction<Aircraft>(AircraftMover(kPlayerAcceleration));

    

    //Shooting mechanics
    m_action_binding[Action::kBulletFire].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
        {
            a.Fire();
        }
    );
    m_action_binding[Action::kMissileFire].action = DerivedAction<Aircraft>([](Aircraft& a, sf::Time dt)
        {
            a.LaunchMissile();
        }
    );

}

bool Player::IsRealTimeAction(Action action)
{
    switch (action)
    {
    case Action::kMoveLeft:
    case Action::kMoveRight:
    case Action::kMoveUp:
    case Action::kMoveDown:
    case Action::kBulletFire:
        return true;
    default:
        return false;
    }
}
