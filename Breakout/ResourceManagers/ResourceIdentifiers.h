#pragma once

// Issue with global objects is that if they want to use another class,
// You need to prototype them here, on lines before the code where you need them

// Prototyped 
namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum class ID
	{
		NormalBall,
		NormalPaddle,
		NormalBrick,
		TitleScreen,
		WorldBG,
	};
}

namespace Fonts
{
	enum ID
	{
		Main,
	};
}

template <typename Resource, typename Identifier>
class ResourceHolder; // prototype it so we can call the typedef on it

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder; // Makes a TextureHolder that only holds Texture Resources and accepts only Texture Id's
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;