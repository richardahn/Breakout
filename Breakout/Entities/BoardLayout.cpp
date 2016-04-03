
#include "BoardLayout.h"

BoardLayout::BoardLayout(float x, float y, float boardWidth, float boardHeight, int numBricksInRow, int numBricksInCol)
	: mLeft(x)
	, mTop(y)
	, mBoardWidth(boardWidth)
	, mBoardHeight(boardHeight)
	, mNumBricksInRow(numBricksInRow)
	, mNumBricksInCol(numBricksInCol)
{

}

void BoardLayout::init(TextureHolder& textures)
{
	mBricks.reserve(mNumBricksInCol * mNumBricksInRow);
	float brickWidth = mBoardWidth / mNumBricksInRow;
	float brickHeight = mBoardHeight / mNumBricksInCol;
	for (int i = 0; i < mNumBricksInCol; i++)
	{
		for (int j = 0; j < mNumBricksInRow; j++)
		{
			float x = mLeft + i * brickWidth;
			float y = mTop + j * brickHeight;
			sf::IntRect brickTextureBounds(0, 0, brickWidth, brickHeight);
			Brick * newBrick = new Brick(Brick::Type::Normal, textures, brickTextureBounds);
			newBrick->setPosition(x, y);
			mBricks.push_back(std::move(std::unique_ptr<Brick>(newBrick)));
		}
	}
}

std::vector<std::unique_ptr<Brick>>& BoardLayout::getBoardEntities()
{
	return mBricks;
}