#pragma once

#include "Entity.h"
#include "Paddle.h"
#include "Brick.h"
#include "../ResourceManagers/ResourceIdentifiers.h"

class Ball : public Entity
{
public:
	enum class Type 
	{
		Normal,
	};
public:
	Ball(Type type, const TextureHolder& textures);
	static sf::Vector2f getSpeedComponents(float speed, float directionFactorX, float directionFactorY);
	static sf::Vector2f getSpeedComponents(float speed, sf::Vector2f directionFactor);


	sf::FloatRect getBoundingRect() const;
	void collidesWith(Entity* other, sf::Time dt);
private:
	static sf::Vector2f getReflectionVelocity(Ball * ball, Paddle * paddle);

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt);
private:
	Type mType;
	sf::Sprite mSprite;
	
};