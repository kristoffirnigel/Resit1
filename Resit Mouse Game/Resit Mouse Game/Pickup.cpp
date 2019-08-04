#include "stdafx.h"
#include "Pickup.h"

Pickup::Pickup(int type) //setting up 3 different types for the 3 different pickups: cheese poison and trap
{
	m_Type = type;
	if (m_Type == 1)
	{
		m_Texture.loadFromFile("graphics/cheese.png");
		m_Sprite.setTexture(m_Texture);
		m_Value = 1;
	}
	else if (m_Type == 2)
	{
		m_Texture.loadFromFile("graphics/poison.png");
		m_Sprite.setTexture(m_Texture);
		m_Value = 2;
	}
	else
	{
		m_Texture.loadFromFile("graphics/trap.png");
		m_Sprite.setTexture(m_Texture);
	}
	m_Sprite.setOrigin(90, 90); 

	m_SecondsLeft = START_COUNTDOWN;
	m_SecondsToWait = START_WAIT_TIME; //the time between spawns and selfdestructions
}
void Pickup::setArena(IntRect arena) //telling the class the border within which it can spawn (doesn`t seem to work tho)
{
	m_Arena.left = arena.left + 180;
	m_Arena.width = arena.width - 180;
	m_Arena.top = arena.top + 180;
	m_Arena.height = arena.height - 180;

	spawn();
}
void Pickup::spawn() //random number generator uses info from the previous function for spawning
{
 	int x = (rand() % m_Arena.width);
	int y = (rand() % m_Arena.height);
	m_SecondsSinceSpawn = 0;
	m_Spawned = true;
	m_Sprite.setPosition(x, y);
}
FloatRect Pickup::getPosition()
{
	return m_Sprite.getGlobalBounds();
}
Sprite Pickup::getSprite()
{
	return m_Sprite;
}
bool Pickup::isSpawned()
{
	return m_Spawned;
}
int Pickup::gotIt() //this is the function we call once the pickup colision is detected in main
{
	m_Spawned = false;
	m_SecondsSinceDestroy = 0;
	return m_Value;
}
void Pickup::update(float elapsedTime) //and here is the timer to tell the pickups to spawn and selfdestruct
{
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elapsedTime;
	}
	else
	{
		m_SecondsSinceDestroy += elapsedTime;
	}
	if (m_SecondsSinceSpawn > m_SecondsLeft && m_Spawned)
	{
		m_Spawned = false;
		m_SecondsSinceDestroy = 0;
	}
	if (m_SecondsSinceDestroy > m_SecondsToWait && !m_Spawned)
	{
		spawn();
	}
}