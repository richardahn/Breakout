#pragma once

#include "State.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Text.hpp>
#include <vector>

class MenuState : public State
{
public:
	enum OptionNames
	{
		Play,
		Exit,
	};
public:
	MenuState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

	// Highlights currently selected option
	void updateOptionText();

private:
	sf::Sprite mBackgroundSprite;

	std::vector<sf::Text> mOptions;
	std::size_t mOptionIndex;
};