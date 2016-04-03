#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>

// SceneNodes are drawable, so we extend Drawable, 
// we also wish to have Transformable
class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> Ptr;
public:
	SceneNode();

	void attachChild(Ptr child);
	Ptr detachChild(const SceneNode& node);

	void update(sf::Time dt);

	sf::Transform getWorldTransform() const;
	sf::Vector2f getWorldPosition() const;



private:
	// "Prefer to make virtual functions private": no need to make them anything but private
	virtual void updateCurrent(sf::Time dt);
	void updateChildren(sf::Time dt);

	// should be final
	virtual void draw(sf::RenderTarget& target, // Note: draw is called by the window, not you; the window can draw any drawable obj
		sf::RenderStates states) const; // states contains the transform member which holds the info for where to render the parent
										// along with the current node's transform, which determines the pos,rot,scale relative to parent node
										//, we can calculate the abs location automatically

										// RenderStates is passed by value since we want a copy of the parent
										// so we can pass in the details to draw, we dont want to change the details of the parent, just a copy
	virtual void drawCurrent(sf::RenderTarget& target,
		sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const; // Not virtual since the implementation
																				// shouldn't change. It's like a helper
																				// for draw(), which is only inherited once down the line, it should be final
																				// drawCurrent is the only thing that should be overriden, because it is the actual drawing process


	std::vector<Ptr> mChildren;
	SceneNode* mParent;
};