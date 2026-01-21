#pragma once
enum class Action
{
	//player 1 controls
	kMoveLeft,
	kMoveRight,
	kMoveUp,
	kMoveDown,
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