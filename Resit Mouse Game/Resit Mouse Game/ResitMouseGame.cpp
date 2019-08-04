#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "ResitMouseGame.h"
#include "Pickup.h"

using namespace sf;

int main()
{
	enum class State //here we create a class for different states that the game can be inat a time
	{
		PAUSED, MENU, GAME_OVER, PLAYING
	};
	State state = State::MENU; //the game will start in Menu state

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width; //used VideoMode to get the desktop resolution and set up the window
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "Resit Mouse Game", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y)); //here we set up the main view from the top corner

	Clock clock;
	Time gameTimeTotal;

	Player player; //instance of the player class

	IntRect arena; //here we create a background
	VertexArray background;
	Texture textureBackground; 
	textureBackground.loadFromFile("graphics/background.png");

	Pickup cheesePickup(1); //all three tpes of pickups
	Pickup poisonPickup(2);
	Pickup trapPickup(3);

	int score = 0; //score and high score that is ment toincreas and decreas with pickups
	int highScore = 0;

	Sprite spriteMenu; //sprites for the manu and the game over screens
	Texture textureMenu;
	textureMenu.loadFromFile("graphics/menu.png");
	spriteMenu.setTexture(textureMenu);
	spriteMenu.setPosition(0, 0);

	Sprite spriteGameOver;
	Texture textureGameOver;
	textureGameOver.loadFromFile("graphics/gameOver.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y)); //new view for hud!

	Font font; //loading font from file
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	Text pausedText; //creating hud elements, score signs on the side, paused sign in the midle
	pausedText.setFont(font);
	pausedText.setCharacterSize(90);
	pausedText.setPosition(720, 540);
	pausedText.setString("PAUSED");

	Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setCharacterSize(25);
	highScoreText.setPosition(1100, 0);
	std::stringstream s;
	s << "HIGH SCORE:" << highScore;
	highScoreText.setString(s.str());

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(25);
	scoreText.setPosition(1100, 180);

	std::ifstream inputFile("gameData/scores.txt"); //for highscore
	if (inputFile.is_open())
	{
		inputFile >> highScore;
		inputFile.close();
	}
	
	int lastHudUpdate = 0; 
	int fpsInterval = 1000; //for hud update checks 

	SoundBuffer biteBuffer; //sounds called in from files
	biteBuffer.loadFromFile("sounds/bites.wav");
	Sound bite;
	bite.setBuffer(biteBuffer);

	SoundBuffer dieBuffer;
	dieBuffer.loadFromFile("sounds/dies.wav");
	Sound die;
	die.setBuffer(dieBuffer);

	SoundBuffer musicBuffer;
	musicBuffer.loadFromFile("sounds/themeMusic.wav");
	Sound music;
	music.setBuffer(musicBuffer);

	while(window.isOpen()) //HERE COMES THE MAIN GAME LOOP
	{
		/*INPUT*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::MENU)
				{
					state = State::PLAYING;
				}
				else if (event.key.code == Keyboard::Return && state == State::PLAYING)
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
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) //this is for quitting
		{
			window.close();
		}
		if (state == State::PLAYING) //keyboard controlls, WASD, the little mouse sprite will move like a toy car
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

				int tileSize = createBackground(background, arena); //one tile is 180*180, game will assemble the background from them

				score = 0; //score to zero

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

			player.update(dtAsSeconds, Mouse::getPosition()); //here we call the function that moves the player

			Vector2f playerPosition(player.getCenter());   

			cheesePickup.update(dtAsSeconds);
			poisonPickup.update(dtAsSeconds);
			trapPickup.update(dtAsSeconds);

			if (player.getPosition().intersects(cheesePickup.getPosition()) && cheesePickup.isSpawned())
			{
				player.bite(gameTimeTotal); /***********attempt to fix the skyrocketing scores*******************************/
				score += cheesePickup.gotIt();
				bite.play();
			}
			if (player.getPosition().intersects(poisonPickup.getPosition()) && poisonPickup.isSpawned())
			{
				player.bite(gameTimeTotal);/***************************did not seem to work though***************************/
				score -= poisonPickup.gotIt();
				bite.play();
			}
			if (player.getPosition().intersects(trapPickup.getPosition()) && trapPickup.isSpawned())
			{
				player.bite(gameTimeTotal); /************************didn`t break nothing either*****************************/
				score = score;
				bite.play();
				state = State::GAME_OVER;
				std::ofstream outputFile("gameData/score.txt"); //calling the highscore
				outputFile << highScore;
				outputFile.close();
			}
			if (score >= highScore)
			{
				highScore = score;
			}

			lastHudUpdate++;
			if (lastHudUpdate > fpsInterval) //if the time of the last hud update was longer than fpsI. it`s time for new update
			{
				std::stringstream ssScore;
				std::stringstream ssHighScore;

				ssScore << "YOUR SCORE:" << score;
				scoreText.setString(ssScore.str());

				ssHighScore << "HIGH SCORE:" << highScore;
				highScoreText.setString(ssHighScore.str());

				lastHudUpdate = 0; //and set the update clock back
			}

		}
		
		//updates end here
		  
		  /*HERE WE DRAW WHAT'S ON SCREEN*/

		if (state == State::PLAYING)
		{
			window.clear();

			window.setView(mainView);

			window.draw(background, &textureBackground);

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
			window.setView(mainView);
			window.draw(spriteMenu);
		}
		if (state == State::PAUSED)
		{
			window.setView(hudView);
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER)
		{
			window.setView(mainView);
			window.draw(spriteGameOver);
			window.draw(scoreText);
			window.draw(highScoreText);
		}
		music.play(); //play the background music
		window.display();

	} 
	//THE MAIN GAME LOOP ENDS HERE
	return 0;
}

