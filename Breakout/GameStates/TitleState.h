

#include "State.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Text.hpp>

class TitleState : public State
{
public:
	TitleState(StateStack& stack, Context context);
	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	static const sf::Time blinkTime;

	sf::Sprite mBackgroundSprite;
	sf::Text mTitle;
	sf::Text mInstructions;

	bool mShowText;
	sf::Time mTextEffectTime;
};