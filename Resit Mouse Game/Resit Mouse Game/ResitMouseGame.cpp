// included things libraries etc
#include "stdafx.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
	VideoMode vm(1920, 1080);
	RenderWindow window(vm, "Resit Mouse Game", Style::Fullscreen); //this makes the game play fullscreen

	Texture textureBackground; //created a texture
	textureBackground.loadFromFile("graphics/background.png"); //added an image file to it
	Sprite spriteBackground; //created a sprite
	spriteBackground.setTexture(textureBackground); //added texture with the image to this sprite
	spriteBackground.setPosition(0, 0); //and now it covers the whole screen

	while(window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		window.clear(); //clears the last frme
		window.draw(spriteBackground); //draws the game
		window.display(); //displays the drawn game
	}
	return 0;
}

