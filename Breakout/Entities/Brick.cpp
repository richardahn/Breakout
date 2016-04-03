

#include "Brick.h"
#include "../ResourceManagers/ResourceHolder.h"


Textures::ID toTextureID(Brick::Type type)
{
	switch (type)
	{
	case Brick::Type::Normal:
		return Textures::ID::NormalBrick;
	}
	return Textures::ID::NormalBrick;
}

Brick::Brick(Type type, const TextureHolder& texture)
	: mType(type)
	, mSprite(texture.get(toTextureID(type)))
	, active(true)
{

}

Brick::Brick(Type type, TextureHolder& texture, const sf::IntRect& rect)
	: mType(type)
	, active(true)
	, mSprite()
{
	sf::Texture& brickTexture = texture.get(toTextureID(type));
	brickTexture.setRepeated(true);
	mSprite = sf::Sprite(brickTexture, rect);
}


void Brick::drawCurrent(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	if (active)
		target.draw(mSprite, states);
}

void Brick::updateCurrent(sf::Time dt)
{

}


sf::FloatRect Brick::getBoundingRect() const
{
	sf::FloatRect t = mSprite.getLocalBounds();
	t.left += getPosition().x;
	t.top += getPosition().y;
	return t;
}

bool Brick::isActive() const
{
	return active;
}

void Brick::setActive(bool a)
{
	active = a;
}