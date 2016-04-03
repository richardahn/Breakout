#include "Application.h"
#include "GameStates\State.h"
#include "GameStates\StateIdentifiers.h"

#include "GameStates\GameState.h"
#include "GameStates\MenuState.h"
#include "GameStates\PauseState.h"
#include "GameStates\TitleState.h"


const int Application::ScreenWidth = 800;
const int Application::ScreenHeight = 600;
const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application()
	: mWindow(sf::VideoMode(ScreenWidth, ScreenHeight), "Breakout", sf::Style::Close)
	, mTextures()
	, mFonts()
	, mStateStack(State::Context(mWindow, mTextures, mFonts))
{
	// Some settings
	mWindow.setKeyRepeatEnabled(false);

	// Load my ResourceHolders
	mFonts.load(Fonts::ID::Main, "..\\Breakout\\Resources\\Sansation.ttf");
	mTextures.load(Textures::ID::TitleScreen, "..\\Breakout\\Resources\\TitleScreen.png");

	// Register the states(this doesn't create them, this just creates a factory of registered states)
	registerStates();

	// Push(create) a state
	mStateStack.requestStatePush(States::Title);
}

void Application::run()
{
	// Create a clock to make the game update by the time elapsed, not by a fixed delta
	sf::Clock clock;

	// Start at a time of 0
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		// Get the elapsed time and add it to the invariant
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			// Check inside this loop, because stack might be empty before update() call
			if (mStateStack.isEmpty())
				mWindow.close();
		}

		render();
	}
}

void Application::processInput()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed)
			mWindow.close();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
}

void Application::render()
{
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());

	mWindow.display();
}


// Register the states that are most likely to be used
void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
}