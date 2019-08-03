#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ResitMouseGame.h"
#include "Pickup.h"

using namespace sf; //makes everything easier

int main()
{
	enum class State //here we create a class for different states that the game can be inat a time
	{
		PAUSED, MENU, GAME_OVER, PLAYING
	};
	State state = State::GAME_OVER; //the game will start in game over state

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

	IntRect arena; //************************HERE IS THE ARENA RECT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//here we create a background
	VertexArray background; //***************HERE IS THE BACKGROUND VERTEX ARRAY!!!!!!!!!!!!!!!!!!!!!
	Texture textureBackground; 
	textureBackground.loadFromFile("graphics/background.png");

	Pickup cheesePickup(1);
	Pickup poisonPickup(2);
	Pickup trapPickup(3);
	
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
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::MENU;
				}
				if (state == State::PLAYING)
				{

				}
			}
		} //here is the end of event management

		if (Keyboard::isKeyPressed(Keyboard::Escape)) //this is for quitting
		{
			window.close();
		}

		if (state == State::PLAYING) //keyboard controlls, pretty straight forward WASD
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

		if (state == State::MENU)
		{
			if (event.key.code == Keyboard::Return)
			{
				state = State::PLAYING;
			}
			if (state == State::PLAYING) //**************************HERE IS THE ARENA GENERATION BETWEEN MENUS!!!!
			{
				arena.width = 1080;
				arena.height = 1080;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createBackground(background, arena);

				player.spawn(arena, resolution, tileSize);

				cheesePickup.setArena(arena);
				poisonPickup.setArena(arena);
				trapPickup.setArena(arena);

				clock.restart();
			}
		}

			/*HERE COME DE UPDATES*/

		if (state == State::PLAYING)
		{
			Time dt = clock.restart();

			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();

			mouseScreenPosition = Mouse::getPosition();  //**********************HERE IS THE MOUSE UPDATE!!!!!!!

			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView); 

			player.update(dtAsSeconds, Mouse::getPosition()); //!!!!!!!!!!!!!!!!!!

			Vector2f playerPosition(player.getCenter()); 

			mainView.setCenter(player.getCenter());  

			cheesePickup.update(dtAsSeconds);
			poisonPickup.update(dtAsSeconds);
			trapPickup.update(dtAsSeconds);

		} //updates end here
		  
		  /*HERE WE DRAW*/

		if (state == State::PLAYING)
		{
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground); //**************BACKGROUND GENERATION!!!!!!!!!!!

			window.draw(player.getSprite());

			if (cheesePickup.isSpawned())
			{
				window.draw(cheesePickup.getSprite());
			}
			if (poisonPickup.isSpawned())
			{
				window.draw(poisonPickup.getSprite());
			}
			if (trapPickup.isSpawned())
			{
				window.draw(trapPickup.getSprite());
			}

		}
		if (state == State::MENU)
		{

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

