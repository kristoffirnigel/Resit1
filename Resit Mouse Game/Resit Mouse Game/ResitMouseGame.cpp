#include "stdafx.h"
#include <sstream>
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

	int score = 0;
	int highScore = 0;

	Sprite spriteMenu;
	Texture textureMenu;
	textureMenu.loadFromFile("graphics/menu");
	spriteMenu.setTexture(textureMenu);
	spriteMenu.setPosition(1080, 0);

	Sprite spriteGameOver;
	Texture textureGameOver;
	textureGameOver.loadFromFile("graphics/gameOver.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(1080, 0);

	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y)); //new view for hud!

	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(90);
	pausedText.setPosition(720, 540);
	pausedText.setString("PAUSED");

	Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(25);
	highScoreText.setPosition(1000, 0);
	std::stringstream s;
	s << "HIGH SCORE:" << highScore;
	highScoreText.setString(s.str());

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(25);
	scoreText.setPosition(440, 0);
	
	int lastHudUpdate = 0;
	int fpsInterval = 1000;

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
				else if (event.key.code == Keyboard::Return && state == State::MENU)
				{
					state = State::PLAYING;
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

			/*HERE COME THE UPDATES*/

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

			if (player.getPosition().intersects(cheesePickup.getPosition()) && cheesePickup.isSpawned())
			{
				score += cheesePickup.gotIt();
				if (score >= highScore)
				{
					highScore = score;
				}
			}
			if (player.getPosition().intersects(poisonPickup.getPosition()) && poisonPickup.isSpawned())
			{
				score -= poisonPickup.gotIt();
			}
			if (player.getPosition().intersects(trapPickup.getPosition()) && trapPickup.isSpawned())
			{
				state = State::GAME_OVER;
			}

			lastHudUpdate++;
			if (lastHudUpdate > fpsInterval)
			{
				std::stringstream ssScore;
				std::stringstream ssHighScore;

				ssScore << "YOUR SCORE:" << score;
				scoreText.setString(ssScore.str());

				ssHighScore << "HIGH SCORE:" << highScore;
				highScoreText.setString(ssHighScore.str());

				lastHudUpdate = 0;
			}

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

			window.setView(hudView);

			window.draw(scoreText);
			window.draw(highScoreText);

		}
		if (state == State::MENU)
		{
			window.draw(spriteMenu);
		}
		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(scoreText);
			window.draw(highScoreText);
		}

		window.display();

	} //THE MAIN GAME LOOP ENDS HERE
	return 0;
}

