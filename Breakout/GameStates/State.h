#pragma once

#include "StateIdentifiers.h"
#include "../ResourceManagers/ResourceIdentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

// Forward declarations
namespace sf
{
	class RenderWindow;
}
class StateStack;
class Player;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	// Context is placed here since a State is responsible for using a Context, it only makes sense on a State
	// That's why its not initialized in the StateStack
	struct Context
	{
							Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts);

		// Why pointers? TODO
		sf::RenderWindow*	window;
		TextureHolder*		textures;
		FontHolder*			fonts;
	};


public:
						State(StateStack& stack, Context context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(sf::Time dt) = 0;
	virtual bool		handleEvent(const sf::Event& event) = 0;


protected:
	// Also I wrap these in a requester function since mStack is rightfully private
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;


private:
	// Holds a StateStack since a State can ALSO determine the flow of states
	// It just calls the push/pop/clear operations on it
	StateStack*			mStack;
	Context				mContext;
};
