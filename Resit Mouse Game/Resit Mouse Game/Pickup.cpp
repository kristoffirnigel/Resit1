#include "stdafx.h"
#include "Pickup.h"

Pickup::Pickup(int type)
{
	m_Type = type;
	if (m_Type == 1)
	{
		m_Texture.loadFromFile("graphics/cheese.png");
		m_Sprite.setTexture(m_Texture);
		m_Value = GROTH_RATE;
	}
	else
	{
		m_Texture.loadFromFile("graphics/poison.png");
		m_Sprite.setTexture(m_Texture);
		m_Value = SHRINK_RATE;
	}
	m_Sprite.setOrigin(90, 90);

	m_SecondsLeft = START_COUNTDOWN;
	m_SecondsToWait = START_WAIT_TIME;
}
void Pickup::setArena(IntRect arena)
{
	m_Arena.left = arena.left + 180;
	m_Arena.width = arena.width - 180;
	m_Arena.top = arena.top + 180;
	m_Arena.height = arena.height - 180;

	spawn();
}
void Pickup::spawn()
{
	srand((int)time(0) / m_Type);
	int x = (rand() % m_Arena.width);
	srand((int)time(0) * m_Type);
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
int Pickup::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDestroy = 0;
	return m_Value;
}
void Pickup::update(float elapsedTime)
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
void Pickup::change()
{
	if (m_Type == 1)
	{
		m_Value += (GROTH_RATE * .5);
	}
	else
	{
		m_Value += (SHRINK_RATE * .5);
	}
}