#pragma once

#include "ResourceManagers\ResourceHolder.h"
#include "ResourceManagers\ResourceIdentifiers.h"
#include "GameStates\StateStack.h"

#include <SFML\System\Time.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\Text.hpp>

/*
(The Main.cpp file creates a single Application object and runs it, thats it)
The Application class controls input/logic updates and rendering
An Application contains a state stack that 
*/
class Application
{
public:
							Application();
	void					run();


private:
	// Almost all the methods here are private since everything should be called within run()
	// The only thing Main.cpp should call is run(), and run() should handle everything
	void					processInput();
	void					update(sf::Time dt);
	void					render();

	// The application should be responsible for registering the states
	void					registerStates();


private:
	static const int		ScreenWidth;
	static const int		ScreenHeight;
	static const sf::Time	TimePerFrame;

	// The Application should create the main RenderWindow that gets passed down
	sf::RenderWindow		mWindow;
	TextureHolder			mTextures;
	FontHolder				mFonts;

	StateStack				mStateStack;
};