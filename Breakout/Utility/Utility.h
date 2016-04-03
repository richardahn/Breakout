#pragma once

#include <sstream>

// Forward declaration
namespace sf
{
	class Sprite;
	class Text;
	class Time;
}
class Entity;

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void backtrackEntity(Entity * e, sf::Time dt);
