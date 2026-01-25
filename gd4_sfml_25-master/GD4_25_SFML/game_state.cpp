#include "game_state.hpp"
#include "mission_status.hpp"

GameState::GameState(StateStack& stack, Context context) : State(stack, context), m_world(*context.window, *context.fonts), m_player(*context.player), m_player_2(*context.player_2)
{

}

void GameState::Draw()
{
	m_world.Draw();
}

bool GameState::Update(sf::Time dt)
{
	

	if (!m_world.HasAlivePlayer())
	{
		m_player.SetMissionStatus(MissionStatus::kMissionFailure);
		RequestStackPush(StateID::kGameOver);
	}
	else if (m_world.HasPlayerReachedEnd())
	{
		m_player.SetMissionStatus(MissionStatus::kMissionSuccess);
		RequestStackPush(StateID::kGameOver);
	}
	//Player1
	CommandQueue& commands = m_world.GetCommandQueue();
	m_player.HandleRealTimeInput(commands);
	//Player2
	
	m_player_2.HandleRealTimeInput(commands);

	m_world.Update(dt);

	return true;
}

bool GameState::HandleEvent(const sf::Event& event)
{
	//Separated player 1 and player 2
	CommandQueue& commands = m_world.GetCommandQueue();
	

	m_player.HandleEvent(event, commands);
	m_player_2.HandleEvent(event, commands);


	//Escape should bring up the pause menu
	const auto* keypress = event.getIf<sf::Event::KeyPressed>();
	if(keypress && keypress->scancode == sf::Keyboard::Scancode::Escape)
	{
		RequestStackPush(StateID::kPause);
	}
	return true;
}


