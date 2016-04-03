
#include "SceneNode.h"
#include <algorithm>
#include <cassert>

SceneNode::SceneNode()
	: mChildren()
	, mParent(nullptr)
{}

void SceneNode::attachChild(Ptr child)
{
	child->mParent = this;
	mChildren.push_back(std::move(child));
}

// Remember SceneNode::Ptr not Ptr, except when in parameter definition
SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
	auto found = std::find_if(mChildren.begin(), mChildren.end(),
		[&](Ptr& p) -> bool { return p.get() == &node; });
	assert(found != mChildren.end()); // LOGIC ERROR
	Ptr result = std::move(*found);
	result->mParent = nullptr;
	mChildren.erase(found);
	return result;
}

void SceneNode::draw(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	// alignment(not really updating, we are just aligning the relative positions)
	// this isn't the draw
	states.transform *= getTransform();
	// ACTUAL DRAWING
	drawCurrent(target, states); // draw current SceneNode
								 // draw children
	drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	// Nothing by default, gets implemented by subclasses
}

void SceneNode::drawChildren(sf::RenderTarget& target,
	sf::RenderStates states) const
{
	for (const Ptr& child : mChildren) // draw the children, and give them the absolute location of the current(states)
	{
		child->draw(target, states);
	}
}

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt)
{
	// Do nothing by default
}

void SceneNode::updateChildren(sf::Time dt)
{
	for (Ptr& child : mChildren)
		child->update(dt);
}


/*
Entity overrides updateCurrent() since it knows velocity and position and thats all you need to move
Aircraft overrides drawCurrent() since an Entity doesnt have a texture, only a subclass of Entity does
*/

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;
	for (const SceneNode * node = this; node != nullptr; node = node->mParent)
		transform = node->getTransform() * transform;

	return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	return getWorldTransform() * sf::Vector2f();
}
