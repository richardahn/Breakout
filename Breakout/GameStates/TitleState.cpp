#include "TitleState.h"

#include "..\ResourceManagers\ResourceHolder.h"
#include "..\Utility\Utility.h"

#include <SFML\Graphics\RenderWindow.hpp> // For incomplete type not allowed error on window

const sf::Time TitleState::blinkTime = sf::seconds(0.5f);

TitleState::TitleState(StateStack& stack, Context context)
	: State(stack, context)
	, mInstructions()
	, mTitle()
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::TitleScreen));

	// Set up instructions
	mInstructions.setFont(context.fonts->get(Fonts::Main));
	mInstructions.setString("Press any key to start");
	centerOrigin(mInstructions);
	mInstructions.setPosition(context.window->getView().getSize() / 2.f);

	mTitle.setFont(context.fonts->get(Fonts::Main));
	mTitle.setString("Breakout");
	mTitle.setCharacterSize(40);
	centerOrigin(mTitle);
	mTitle.setPosition(context.window->getView().getSize() / 2.f - sf::Vector2f(0.f, context.window->getView().getSize().y / 5.f));
}

void TitleState::draw()
{
	sf::RenderWindow& window = *(getContext().window);
	// Draw background first
	window.draw(mBackgroundSprite);
	window.draw(mTitle);
	// Then text
	if (mShowText)
		window.draw(mInstructions);
}

bool TitleState::handleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed)
	{
		requestStackPop();
		requestStackPush(States::Menu);
	}
	return true;
}

bool TitleState::update(sf::Time dt)
{
	// add to the amount of time elapsed
	mTextEffectTime += dt;

	// If the amount of time exceeds the threshold(blinkTime), we blink and reset the timer
	if (mTextEffectTime >= blinkTime)
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}

	return true;
}



