#pragma once

#include "SceneNode.h"

// SpriteNode refers to the fact that it contains a sprite and thats all(its used for backgrounds)
// every texture can be set to repeated setRepeated(bool)
// basically the actual sprite will stay the same in size, butt if you request a texture rectangle
// that is larger than the texture's real dimensions, it will repeat and expand to fill that larger size
class SpriteNode : public SceneNode
{
public:
	explicit SpriteNode(const sf::Texture& texture);
	SpriteNode(const sf::Texture& texture, const sf::IntRect& rect); // specifies the size of the sprite node
private:
	virtual void drawCurrent(sf::RenderTarget& target,
		sf::RenderStates states) const;
private:
	sf::Sprite mSprite;
};