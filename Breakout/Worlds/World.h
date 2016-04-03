#pragma once

#include "../ResourceManagers/ResourceIdentifiers.h"
#include "../ResourceManagers/ResourceHolder.h"
#include "../Entities/SceneNode.h"
#include "../Entities/SpriteNode.h"
#include "../Entities/Ball.h"
#include "../Entities/Paddle.h"
#include "../Entities/BoardLayout.h"

#include <array>
#include <SFML/Graphics.hpp>
#include <list>

// SceneGraph for holding entities
// CollisionManager for managing collisions
// Enum and array for rendering certain layers first
// GameStates 

// Forward declaration
namespace sf
{
	class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
	explicit World(sf::RenderWindow& window);
	void update(sf::Time dt);
	void draw();

private:
	void processInput();
	void processCollisions(sf::Time dt);
	void setBallOnPaddle();

	sf::FloatRect getViewBounds() const;


	void loadTextures();
	void buildScene();

private:
	enum Layer // I do not use a strongly typed enum here, because I want the LayerCount to convert to an int to represent the # of things in the enum
	{	// and also, I want to use each enum as an int to access the SceneLayers array
		Background,
		Board,
		LayerCount // Not a Layer, only used for knowing the count
	};

private:

	sf::RenderWindow& mWindow;
	// contains the current view of the world(bounded by mWorldBounds)
	sf::View mWorldView; 
	TextureHolder mTextures;
	
	SceneNode mSceneGraph;
	std::array<SceneNode*, LayerCount> mSceneLayers;
	Ball* mBall;
	Paddle *mPaddle;
	BoardLayout mBoardLayout;
	std::vector<Brick*> mBricks;

	sf::FloatRect mWorldBounds;
	sf::Vector2f mSpawnPosition;
	static const sf::Vector2f PaddleMoveSpeed;
	static const sf::Vector2f BallMoveDirectionFactor;
	static const float BallMoveSpeed;
};

/*
A world must contain 'render' data
-Reference to the render window(passed from the game Class)
-The world's current view
-the texture holder with all the textures needed inside the world(remember: texture holder is RAII)
-the scene graph
-some pointers to access the scene graph's layer nodes
A world must also contain 'logical' data
-The bounding rectangle of the WORLD, storing its dimensions
-the position where the player's plane appears in the beginning
-the speed with which the world is scrolled
-a pointer to the player's aircraft
*/

// FloatRect's and IntRect's basically store rectangular dimensions with top,left,width,height