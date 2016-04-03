#pragma once

#include "MenuState.h"
#include "../Utility/Utility.h"
#include "../ResourceManagers/ResourceHolder.h"

#include <SFML\Graphics\RenderWindow.hpp>


// Used to convert enum to string
std::string toString(MenuState::OptionNames option)
{
	switch (option)
	{
	case MenuState::OptionNames::Play:
		return "Play";
		break;
	case MenuState::OptionNames::Exit:
		return "Exit";
		break;
	}
}

MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, mOptions()
	, mOptionIndex(0)
{
	// Retrieve necessary resources
	sf::Texture& texture = context.textures->get(Textures::ID::TitleScreen);
	sf::Font& font = context.fonts->get(Fonts::ID::Main);

	// Set up the background
	mBackgroundSprite.setTexture(texture);

	// Set up the play option
	sf::Text playOption;
	playOption.setFont(font);
	playOption.setString(toString(OptionNames::Play));
	centerOrigin(playOption);
	playOption.setPosition(context.window->getView().getSize() / 2.f);
	mOptions.push_back(playOption);

	// Set up the exit option
	sf::Text exitOption;
	exitOption.setFont(font);
	exitOption.setString(toString(OptionNames::Exit));
	centerOrigin(exitOption);
	exitOption.setPosition(playOption.getPosition() + sf::Vector2f(0.f, 30.f));
	mOptions.push_back(exitOption);

	// Set the colors/highlights
	updateOptionText();
}

void MenuState::draw()
{
	sf::RenderWindow& window = *(getContext().window);

	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);

	for (const sf::Text& text : mOptions)
		window.draw(text);
}

bool MenuState::update(sf::Time dt)
{
	return true;
}

void MenuState::updateOptionText()
{
	if (mOptions.empty())
		return;

	// Reset all text colors
	for (sf::Text& text : mOptions)
	{
		text.setColor(sf::Color::White);
	}

	// Highlight just the current one
	mOptions[mOptionIndex].setColor(sf::Color::Red);
}

bool MenuState::handleEvent(const sf::Event& event)
{
	// Only process key presses
	if (event.type != sf::Event::KeyPressed)
		return false;

	// Selected the current option
	if (event.key.code == sf::Keyboard::Return)
	{
		if (mOptionIndex == Play)
		{
			requestStackPop();
			requestStackPush(States::Game);
		}
		else if (mOptionIndex == Exit)
		{
			// TODO: I don't clear here for debugging reasons, I would clear on release 
			requestStackPop();
		}
	}
	// Navigate up the menu
	else if (event.key.code == sf::Keyboard::Up)
	{
		// Decrement/wrap-around back to top
		if (mOptionIndex != 0)
			mOptionIndex--; // decreasing goes up, remember
		else
			mOptionIndex = mOptions.size() - 1;

		// Update colors/highlights
		updateOptionText();
	}
	// Navigate down the menu
	else if (event.key.code == sf::Keyboard::Down)
	{
		// Increment/wrap-around back to bottom
		if (mOptionIndex != mOptions.size() - 1)
			mOptionIndex++;
		else
			mOptionIndex = 0;

		// Update colors/highlights
		updateOptionText();
	}

	return true;
}