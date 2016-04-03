
#include "Ball.h"
#include "../ResourceManagers/ResourceHolder.h"
#include "../Utility/Utility.h"

Textures::ID toTextureID(Ball::Type type)
{
	switch (type)
	{
	case Ball::Type::Normal:
		return Textures::ID::NormalBall;
	}
	return Textures::ID::NormalBall;
}

Ball::Ball(Type type, const TextureHolder& textures)
	: mType(type)
	, mSprite(textures.get(toTextureID(type)))
{
}

void Ball::drawCurrent(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Ball::updateCurrent(sf::Time dt)
{
	move(getVelocity());
}

sf::FloatRect Ball::getBoundingRect() const
{
	sf::FloatRect t = mSprite.getLocalBounds();
	t.left += getPosition().x;
	t.top += getPosition().y;
	return t;
}

// should only be called after an intersection of two entities
void Ball::collidesWith(Entity* other, sf::Time dt)
{
	if (Paddle * pOther = dynamic_cast<Paddle*>(other))
	{
		backtrackEntity(other, dt);
		float distX = this->getVelocity().x * dt.asSeconds();
		float distY = this->getVelocity().y * dt.asSeconds();

		// Iterate one axis at a time
		// Y-axis has priority
		this->move(0.f, distY);
		if (this->getBoundingRect().intersects(pOther->getBoundingRect()))
		{
			this->move(distX, 0.f);
			// If entering from the top side
			if (distY > 0)
			{
				this->setPosition(this->getPosition().x, pOther->getPosition().y - this->getBoundingRect().height);
				float speedMag = sqrt(getVelocity().x * getVelocity().x + getVelocity().y * getVelocity().y);
				this->setVelocity(getSpeedComponents(speedMag, getReflectionVelocity(this, pOther)));
			}
			// If entering from the bot side
			else
			{
				this->setPosition(this->getPosition().x, pOther->getPosition().y + pOther->getBoundingRect().height);
				this->setVelocity(this->getVelocity().x, -this->getVelocity().y);
			}
		}
		else {
			this->move(distX, 0.f);
			if (this->getBoundingRect().intersects(pOther->getBoundingRect()))
			{
				// If entering from the right side
				if (distX < 0)
				{
					this->setPosition(pOther->getPosition().x + pOther->getBoundingRect().width, this->getPosition().y);
					this->setVelocity(-this->getVelocity().x, this->getVelocity().y);
				}
				// If entering from the left side
				else
				{
					this->setPosition(pOther->getPosition().x - this->getBoundingRect().width, this->getPosition().y);
					this->setVelocity(-this->getVelocity().x, this->getVelocity().y);
				}
			}
		}
	}
	else if (Brick * bOther = dynamic_cast<Brick*>(other))
	{
		if (bOther->isActive()) {
			backtrackEntity(other, dt);
			float distX = this->getVelocity().x * dt.asSeconds();
			float distY = this->getVelocity().y * dt.asSeconds();

			// Iterate one axis at a time
			// Y-axis has priority
			this->move(0.f, distY);
			if (this->getBoundingRect().intersects(bOther->getBoundingRect()))
			{
				this->move(distX, 0.f);
				// If entering from the top side
				if (distY > 0)
				{
					this->setPosition(this->getPosition().x, bOther->getPosition().y - this->getBoundingRect().height);
					this->setVelocity(this->getVelocity().x, -this->getVelocity().y);
				}
				// If entering from the bot side
				else
				{
  					this->setPosition(this->getPosition().x, bOther->getPosition().y + bOther->getBoundingRect().height);
					this->setVelocity(this->getVelocity().x, -this->getVelocity().y);
				}
			}
			else {
				this->move(distX, 0.f);
				if (this->getBoundingRect().intersects(bOther->getBoundingRect()))
				{
					// If entering from the right side
					if (distX < 0)
					{
						this->setPosition(bOther->getPosition().x + bOther->getBoundingRect().width, this->getPosition().y);
						this->setVelocity(-this->getVelocity().x, this->getVelocity().y);
					}
					// If entering from the left side
					else
					{
						this->setPosition(bOther->getPosition().x - this->getBoundingRect().width, this->getPosition().y);
						this->setVelocity(-this->getVelocity().x, this->getVelocity().y);
					}
				}
			}
			bOther->setActive(false);
		}
	}
}


sf::Vector2f Ball::getReflectionVelocity(Ball * ball, Paddle * paddle)
{
	// Get the distance from the ball's center to the paddle's center
	float distFromCenter = ball->getPosition().x - paddle->getPosition().x;
	distFromCenter -= paddle->getBoundingRect().width / 2.f;
	// Normalize the distance and increase it by a factor of 2
	distFromCenter /= (2.f * (paddle->getBoundingRect().width / 2.f));
	return sf::Vector2f(distFromCenter, -1);
}

sf::Vector2f Ball::getSpeedComponents(float speed, float directionFactorX, float directionFactorY)
{
	// Get the magnitude of the direction factor
	float mag = sqrt(directionFactorX * directionFactorX + directionFactorY * directionFactorY);
	float factor = speed / mag;
	return sf::Vector2f(factor * directionFactorX, factor * directionFactorY);
}
sf::Vector2f Ball::getSpeedComponents(float speed, sf::Vector2f directionFactor)
{
	float mag = sqrt(directionFactor.x * directionFactor.x + directionFactor.y * directionFactor.y);
	float factor = speed / mag;
	return sf::Vector2f(factor * directionFactor.x, factor * directionFactor.y);
}