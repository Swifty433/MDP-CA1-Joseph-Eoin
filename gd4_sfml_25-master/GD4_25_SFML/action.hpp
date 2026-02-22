//Eoin Hamill D00258444
//Joseph Bryne D00255161

//edited by joseph byrne
#pragma once
enum class Action
{
	//player 1 controls
	kMoveLeft,
	kMoveRight,
	//players can no longer move up and down so these have been commented out for now!
	//kMoveUp,
	//kMoveDown,
	kBulletFire,
	kMissileFire,
	
	//Player 2 controls
	kMoveLeftPlayer2,
	kMoveRightPlayer2,
	kSpawnAlienPlayer2,
	kSpawnAlien2Player2,
	kSpawnAlien3Player2,


	//Used to get the amount of elements in the enum Index 11
	kActionCount
};