#pragma once

#include "Entity.h"
#include "../ResourceManagers/ResourceIdentifiers.h"

class Paddle : public Entity
{
public:
	enum class Type
	{
		Normal,
	};

public:
	Paddle(Type type, const TextureHolder& textures);

	sf::FloatRect getBoundingRect() const;

	bool isSticky();
	void setSticky(bool s);
private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt);

private:
	bool sticky;

	Type mType;
	sf::Sprite mSprite;
};