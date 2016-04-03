#pragma once

#include "../ResourceManagers/ResourceIdentifiers.h"
#include "Brick.h"
#include "SceneNode.h"

#include <memory>

class BoardLayout
{
public:
	BoardLayout(float x, float y, float boardWidth, float boardHeight, int numBricksInRow, int numBricksInCol);

	void init(TextureHolder& textures);
	std::vector<std::unique_ptr<Brick>>& getBoardEntities();

	float mLeft;
	float mTop;
	float mBoardWidth;
	float mBoardHeight;
	int mNumBricksInRow;
	int mNumBricksInCol;

private:
	std::vector<std::unique_ptr<Brick>> mBricks;
	
};