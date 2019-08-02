#pragma once
#include <SFML/Graphics.hpp>
#include "stdafx.h"

using namespace sf;

class Player
{
private:
	const float START_SPEED = 250;

	Vector2f m_Position; //for collisions later
	Sprite m_Sprite; //to draw the mouse
	Texture m_Texture;
	Vector2f m_Resolution;
	IntRect m_Arena;
	int m_TileSize;
	bool m_UpPressed; //for controlls
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	float m_Speed;
	float m_Angle;
	float m_RotationSpeed;

public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	void resetPlayerStats();
	FloatRect getPosition();
	Vector2f getCenter();
	float getRotation();
	Sprite getSprite();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	void update(float elapsedTime, Vector2i mousePosition);
};
