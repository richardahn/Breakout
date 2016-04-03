#include "World.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <algorithm>
#include <cmath>
// actual sprite is drawn half the width to the left of the hitbox

const sf::Vector2f World::PaddleMoveSpeed = sf::Vector2f(5, 0);
const sf::Vector2f World::BallMoveDirectionFactor = sf::Vector2f(0.5, -1);
const float World::BallMoveSpeed = 8;

void World::setBallOnPaddle()
{
	mBall->setPosition(mPaddle->getPosition() + sf::Vector2f(mPaddle->getBoundingRect().width / 2.f - mBall->getBoundingRect().width / 2.f, -mBall->getBoundingRect().height));
}

World::World(sf::RenderWindow& window)
	: mWindow(window)
	, mWorldView(window.getDefaultView())
	, mTextures()
	, mSceneGraph()
	, mSceneLayers()
	, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y)
	, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 8.f)
	, mBoardLayout(0.f, 0.f, mWorldView.getSize().x, mWorldView.getSize().y / 2.f, 10, 10)
	, mBricks()
{
	loadTextures();
	mBoardLayout.init(mTextures);
	buildScene();
}

void World::update(sf::Time dt)
{
	processInput();
	mSceneGraph.update(dt);
	processCollisions(dt);
	// Should replace this with a listener and send it to the Ball 
	if (mPaddle->isSticky())
	{
		setBallOnPaddle();
	}
}

void World::processInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		mPaddle->move(-PaddleMoveSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		mPaddle->move(PaddleMoveSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		mPaddle->setSticky(false);
	}
}

// dt is necessary to backtrack 
void World::processCollisions(sf::Time dt)
{
	// TODO: Consider having a board object and use those dimensions for wall collisions instead of the worldBounds, since the world may have walls which have width and height, so they may cause collisions

	// Check and process paddle-wall collisions
	float paddleLeft = mPaddle->getPosition().x;
	float paddleRight = mPaddle->getPosition().x + mPaddle->getBoundingRect().width;
	// Hit left wall
	if (paddleLeft < mWorldBounds.left)
	{
		mPaddle->setPosition(sf::Vector2f(mWorldBounds.left, mPaddle->getPosition().y));
	}
	// Hit right wall
	else if (paddleRight > mWorldBounds.left + mWorldBounds.width)
	{
		mPaddle->setPosition(sf::Vector2f(mWorldBounds.left + mWorldBounds.width - mPaddle->getBoundingRect().width, mPaddle->getPosition().y));
	}

	// Check and process ball-wall collisions
	float ballLeft = mBall->getPosition().x;
	float ballRight = mBall->getPosition().x + mBall->getBoundingRect().width;
	float ballTop = mBall->getPosition().y;
	float ballBot = mBall->getPosition().y + mBall->getBoundingRect().height;
	// Hit left wall
	if (ballLeft < mWorldBounds.left)
	{
		mBall->setPosition(sf::Vector2f(mWorldBounds.left, mBall->getPosition().y));
		mBall->setVelocity(-mBall->getVelocity().x, mBall->getVelocity().y);
	}
	// Hit right wall
	else if (ballRight > mWorldBounds.left + mWorldBounds.width)
	{
		mBall->setPosition(sf::Vector2f(mWorldBounds.left + mWorldBounds.width - mBall->getBoundingRect().width, mBall->getPosition().y));
		mBall->setVelocity(-mBall->getVelocity().x, mBall->getVelocity().y);
	}
	// Hit top wall
	else if (ballTop < mWorldBounds.top)
	{
		mBall->setPosition(sf::Vector2f(mBall->getPosition().x, mWorldBounds.top));
		mBall->setVelocity(mBall->getVelocity().x, -mBall->getVelocity().y);
	}
	// Hit bot wall(At this point, you should reset the ball)
	else if (ballBot > mWorldBounds.top + mWorldBounds.height)
	{
		// TODO: change the way sticky is handled and pass that responsibility to collision, since you may want a sticky powerup
		mPaddle->setSticky(true);
	}
	// Check and process ball-paddle collisions
	if (mBall->getBoundingRect().intersects(mPaddle->getBoundingRect()))
	{
		mBall->collidesWith(mPaddle, dt);
	}	
	// Check and process ball-brick collisions 
	for (size_t i = 0; i < mBricks.size(); i++)
	{
		if (mBricks[i]->isActive() && mBall->getBoundingRect().intersects(mBricks[i]->getBoundingRect()))
		{
			mBall->collidesWith(mBricks[i], dt);
		}
	}
}


void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
	mTextures.load(Textures::ID::NormalBall, "..\\Breakout\\Resources\\Ball.png");
	mTextures.load(Textures::ID::NormalPaddle, "..\\Breakout\\Resources\\Paddle.png");
	mTextures.load(Textures::ID::NormalBrick, "..\\Breakout\\Resources\\Brick.png");
	mTextures.load(Textures::ID::WorldBG, "..\\Breakout\\Resources\\WorldBG.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& texture = mTextures.get(Textures::ID::WorldBG);
	sf::IntRect textureRect(mWorldBounds);
	texture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
	backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add paddle
	std::unique_ptr<Paddle> paddle(new Paddle(Paddle::Type::Normal, mTextures));
	mPaddle = paddle.get();
	mPaddle->setPosition(mSpawnPosition - sf::Vector2f(mPaddle->getBoundingRect().width / 2.f, mPaddle->getBoundingRect().height / 2.f));
	mPaddle->setSticky(true);
	mSceneLayers[Board]->attachChild(std::move(paddle));

	// Add ball
	std::unique_ptr<Ball> ball(new Ball(Ball::Type::Normal, mTextures));
	mBall = ball.get();
	setBallOnPaddle();
	mBall->setVelocity(Ball::getSpeedComponents(BallMoveSpeed, BallMoveDirectionFactor));
	mSceneLayers[Board]->attachChild(std::move(ball));

	// Add bricks
	auto tempBricks = std::move(mBoardLayout.getBoardEntities());
	mBricks.reserve(mBoardLayout.mNumBricksInCol * mBoardLayout.mNumBricksInRow);
	for (size_t i = 0; i < tempBricks.size(); i++)
	{
		mBricks.push_back(tempBricks[i].get());
		mSceneLayers[Board]->attachChild(std::move(tempBricks[i]));
	}
}

sf::FloatRect World::getViewBounds() const
{
	return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}