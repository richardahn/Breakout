#pragma once

#include "Entity.h"
#include "../ResourceManagers/ResourceIdentifiers.h"


class Brick : public Entity
{
public:
	enum class Type
	{
		Normal,
	};

	Brick(Type type, const TextureHolder& texture);
	Brick(Type type, TextureHolder& texture, const sf::IntRect& rect);

	sf::FloatRect getBoundingRect() const;
	bool isActive() const;
	void setActive(bool a);

private:
	virtual void updateCurrent(sf::Time dt);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	bool active;

	Type mType;
	sf::Sprite mSprite;
};