#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private :
	//const int GROTH_RATE = 0;
	const int START_WAIT_TIME = 0;
	const int START_COUNTDOWN = 5;

	Sprite m_Sprite;
	Texture m_Texture;
	IntRect m_Arena;
	int m_Value;
	int m_Type;

	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDestroy;
	float m_SecondsLeft;
	float m_SecondsToWait;


public:
	Pickup::Pickup(int type);
	void setArena(IntRect arena);
	void spawn();
	FloatRect getPosition();
	Sprite getSprite();
	void update(float elepsedTime);
	bool isSpawned();
	int gotIt();
};