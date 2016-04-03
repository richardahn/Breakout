
#include "Paddle.h"
#include "../ResourceManagers/ResourceHolder.h"
#include "../Utility/Utility.h"

Textures::ID toTextureID(Paddle::Type type)
{
	switch (type)
	{
	case Paddle::Type::Normal:
		return Textures::ID::NormalPaddle;
	}
	return Textures::ID::NormalPaddle;
}

Paddle::Paddle(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(toTextureID(type)))
	, sticky(0)
{
}

void Paddle::drawCurrent(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Paddle::updateCurrent(sf::Time dt)
{
}

bool Paddle::isSticky()
{
	return sticky;
}

void Paddle::setSticky(bool s)
{
	sticky = s;
}

sf::FloatRect Paddle::getBoundingRect() const
{
	sf::FloatRect t = mSprite.getLocalBounds();
	t.left += getPosition().x;
	t.top += getPosition().y;
	return t;
}