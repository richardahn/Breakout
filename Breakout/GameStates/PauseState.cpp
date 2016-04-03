#pragma once

#include "PauseState.h"
#include "../ResourceManagers/ResourceHolder.h"
#include "../Utility/Utility.h"

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\View.hpp>
// TODO: fix the incomplete type problem by including the file in Context itself(State.h)
// only occurs when you want to use a METHOD in the class btw, which is correct


PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, mBackgroundSprite()
	, mPausedText()
	, mInstructionText()
{
	sf::Font& font = context.fonts->get(Fonts::Main);
	sf::Vector2f viewSize = context.window->getView().getSize(); // I need this to center the pausedText

	// Set the paused text
	mPausedText.setFont(font);
	mPausedText.setString("Game Paused");
	mPausedText.setCharacterSize(70);
	centerOrigin(mPausedText);
	mPausedText.setPosition(0.5f * viewSize.x, 0.4f * viewSize.y);

	// Set the paused sub-text
	mInstructionText.setFont(font);
	mInstructionText.setString("(Press Backspace to return to the main menu)");
	centerOrigin(mInstructionText);
	mInstructionText.setPosition(0.5f * viewSize.x, 0.6f * viewSize.y);
}

void PauseState::draw()
{
	sf::RenderWindow& window = *(getContext().window);
	window.setView(window.getDefaultView());

	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mPausedText);
	window.draw(mInstructionText);
}

/*
I'm considering a bool for the State class that basically
determines if the state should be updated or not

Usually, subclasses will make the bool false when pushing another state to the stack
*/
bool PauseState::update(sf::Time)
{
	return false;
}

bool PauseState::handleEvent(const sf::Event& event)
{
	if (event.type != sf::Event::KeyPressed)
		return false;

	// Return back to game
	if (event.key.code == sf::Keyboard::Escape)
	{
		requestStackPop();
	}

	// Quit to menu
	if (event.key.code == sf::Keyboard::BackSpace)
	{
		requestStateClear();
		requestStackPush(States::Menu);
	}

	return false;
}

