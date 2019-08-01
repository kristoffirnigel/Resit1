#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ResitMouseGame.h"

using namespace sf; //makes everything easier

int main()
{
	enum class State //here we create a class for different states that the game can be inat a time
	{
		PAUSED, MENU, GAME_OVER, PLAYING
	};
	State state = State::PAUSED; //the game will start in paused state

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width; //used VideoMode to get the desktop resolution and set up the window
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Resit Mouse Game", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Clock clock;
	Time gameTimeTotal;

	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	Player player; //instance of the player class

	IntRect arena;
	//here we create a background
	VertexArray background;
	Texture textureBackground; 
	textureBackground.loadFromFile("graphics/background.png"); 
	
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

		if (state == State::PLAYING) //keyboard controlls, pretty straight forward
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
		} //inpust end here

		if (state == State::PAUSED)
		{
			if (event.key.code == Keyboard::Return)
			{
				state == State::PLAYING;
			}
			if (state == State::PLAYING)
			{
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);

				clock.restart();
			}
		}

		/*HERE COME DE UPDATES*/

		if (state == State::PLAYING)
		{
			Time dt = clock.restart();

			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();

			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			player.update(dtAsSeconds, Mouse::getPosition());

			Vector2f playerPosition(player.getCenter());

			mainView.setCenter(player.getCenter());
		} //updates end here

		/*HERE WE DRAW*/

		if (state == State::PLAYING)
		{
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);
			
			window.draw(player.getSprite());
		}
		if (state == State::PAUSED)
		{

		}
		if (state == State::GAME_OVER)
		{

		}

		window.display();

	} //THE MAIN GAME LOOP ENDS HERE
	return 0;
}

