#include "game_state.hpp"
#include "mission_status.hpp"

GameState::GameState(StateStack& stack, Context context) : State(stack, context), m_world(*context.window, *context.fonts, *context.audio), m_player(*context.player), m_player_2(*context.player_2)
{
	context.audio->play_music(Music::kLevelMusic);
}

void GameState::Draw()
{
	m_world.Draw();
}

bool GameState::Update(sf::Time dt)
{

	//if (!m_world.HasAlivePlayer())
	//{
	//	m_player.SetMissionStatus(MissionStatus::kMissionFailure);
	//	RequestStackPush(StateID::kGameOver);
	//}
	//else if (m_world.HasPlayerReachedEnd())
	//{
	//	m_player.SetMissionStatus(MissionStatus::kMissionSuccess);
	//	RequestStackPush(StateID::kGameOver);
	//}

	//checking to see who the winner is and then displaying the appropriate game over screen
	if (m_world.HasGameEnded())
	{
		int winner = m_world.GetWinner();
		if (winner == 1)
		{
			m_player.SetMissionStatus(MissionStatus::kPlayer1Wins);
		}
		else if (winner == 2)
		{
			m_player.SetMissionStatus(MissionStatus::kPlayer2Wins);
		}
		RequestStackPush(StateID::kGameOver);
	}

	CommandQueue& commands = m_world.GetCommandQueue();
	//Player1
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


