#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf; //makes everything easier

int main()
{
	enum class State
	{
		PAUSED, GAME_OVER, PLAYING
	};
	State state = State::PLAYING;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Resit Mouse Game", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;

	Vector2f moseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player;

	IntRect arena;

	Texture textureBackground; //created a texture
	textureBackground.loadFromFile("graphics/background.png"); //added an image file to it
	Sprite spriteBackground; //created a sprite
	spriteBackground.setTexture(textureBackground); //added texture with the image to this sprite
	spriteBackground.setPosition(0, 0); //and now it covers the whole screen cuz they are the same size yey

	while(window.isOpen()) //HERE COMES THE MAIN GAME LOOP
	{
		/*INPUT*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}
				if (state == State::GAME_OVER)
				{

				}
			}
		} //here is the end of event management

		if (Keyboard::isKeyPressed(Keyboard::Escape)) //this is for quitting
		{
			window.close();
		}

		if (state == State::PLAYING)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
		}

		/*HERE COME DE UPDATES*/

		if (state == State::PLAYING)
		{
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();

			moseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());
		}

		/*HERE WE DRAW*/

		if (state == State::PLAYING)
		{
			window.clear();
			window.setView(mainView);
			window.draw(spriteBackground);
			window.draw(player.getSprite());
		}

		window.display();

	} //THE MAIN GAME LOOP ENDS HERE
	return 0;
}

