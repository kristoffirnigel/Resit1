#include "stdafx.h"
#include "player.h"

Player::Player()
{
	m_Speed = START_SPEED; // this needs Size addad!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_Angle = 0;
	m_RotationSpeed = 100;
	m_Texture.loadFromFile("graphics/mouse.png");
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setOrigin(74, 110);
}
void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	m_TileSize = tileSize;
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}
void Player::resetPlayerStats() // this here is for when we respawn the player after death (needs Size added!)
{
	m_Speed = START_SPEED;
}
FloatRect Player::getPosition()
{
	return m_Sprite.getGlobalBounds();
}
Vector2f Player::getCenter()
{
	return m_Position;
}
float Player::getRotation()
{
	return m_Sprite.getRotation();
}
Sprite Player::getSprite()
{
	return m_Sprite;
}
void Player::moveLeft()
{
	m_LeftPressed = true;
}
void Player::moveRight()
{
	m_RightPressed = true;
}
void Player::moveUp()
{
	m_UpPressed = true;
}
void Player::moveDown()
{
	m_DownPressed = true;
}
void Player::stopLeft()
{
	m_LeftPressed = false;
}
void Player::stopRight()
{
	m_RightPressed = false;
}
void Player::stopUp()
{
	m_UpPressed = false;
}
void Player::stopDown()
{
	m_DownPressed = false;
}
void Player::update(float elapsedTime, Vector2i mousePosition)
{
	float speed = 0;
	if (m_UpPressed)
	{
		speed -= m_Speed * elapsedTime;
	}
	if (m_DownPressed)
	{
		speed += m_Speed * elapsedTime;
	}
	if (m_LeftPressed)
	{
		m_Angle -= m_RotationSpeed * elapsedTime;
		if (m_Angle < 0)
		{
			m_Angle += 0;
		}
	}
	if (m_RightPressed)
	{
		m_Angle += m_RotationSpeed * elapsedTime;
		if (m_Angle > 360)
		{
			m_Angle -= 360;
		}
	}
	float radian = m_Angle / 180 * 3.141;

	m_Position.x += speed * cos(radian);
	m_Position.y += speed * sin(radian);

	/*if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elapsedTime;
	}
	if (m_DownPressed)
	{
		m_Position.y += m_Speed * elapsedTime;
	}
	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elapsedTime;
	}
	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elapsedTime;
	}*/

	m_Sprite.setRotation(m_Angle - 90);

	m_Sprite.setPosition(m_Position);

	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}
	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}
	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}
	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	//float angle = (atan2(mousePosition.y - m_Resolution.y / 2, mousePosition.x - m_Resolution.x / 2) * 180) / 3.141;

	//m_Sprite.setRotation(m_Angle);
}












