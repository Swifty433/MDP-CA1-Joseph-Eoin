#pragma once
enum class ReceiverCategories
{
	kNone = 0,
	kScene = 1 << 0,
	
	kPlayerAircraft = 1 << 1,
	kPlayer1Aircraft = 1 << 2,
	kPlayer2Aircraft = 1 <<3,

	kAlliedAircraft = 1 << 4,
	kEnemyAircraft = 1 << 5, 
	kAlliedProjectile = 1 << 6,
	kEnemyProjectile = 1 << 7,
	kPickup = 1 << 8,

	kAircraft = kPlayerAircraft | kAlliedAircraft | kEnemyAircraft,
	kProjectile = kAlliedProjectile | kEnemyProjectile
};

//A message that would be sent to all aircraft would be
//unsigned int all_aircraft = ReceiverCategories::kPlayer | ReceiverCategories::kAlloedAircraft | ReceiverCategories::kEnemyAircraft