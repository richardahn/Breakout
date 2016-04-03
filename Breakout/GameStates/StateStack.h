#pragma once

#include "StateIdentifiers.h"
#include "State.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

// Forward declaration
namespace sf
{
	class Event;
	class RenderWindow;
}

/*
What is a StateStack
A StateStack has an enum called Action
A StateStack needs to have this enum because I want to make 
REQUESTS to it and not direct actions, so I use an enum so I can reference
a specific action later on
I then store my requests in a list. BUT WAIT,
for every request(push in particular), you need to know what state youre pushing
so when you request a push, you need an associated state, and you should have an ID instead
since you need an ID to CREATE a state
*/
class StateStack : private sf::NonCopyable
{
public:
	// should be public since 
	enum Action
	{
		Push,
		Pop,
		Clear,
	};
public:
	// A StateStack holds a Context object for all states to use
	// The Context object holds references to resources and progression of states
	explicit				StateStack(State::Context context);

	// Registers(Associates) a stateID with a specific state
	template <typename T>
	void					registerState(States::ID stateID);

	// Updates all the stacks in order from top to bottom
	void					update(sf::Time dt);
	// Draws all the stacks from bottom to top(so that the peek is drawn last)
	void					draw();
	// Gives the event to each state so they can handle it
	void					handleEvent(const sf::Event& event); 

	// These 3 methods only add PENDING changes, the effects are not immediate and only apply
	// once applyPendingChanges has been called
	void					requestStatePush(States::ID stateID);
	void					requestStatePop();
	void					requestStatesClear();

	bool					isEmpty() const;
private:
	// Uses the map with registered ID's to create a state
	State::Ptr				createState(States::ID stateID);
	// Goes through the vector of requested changes and processes them at a safe point in time
	// An unsafe point in time would be pushing a stack while we're doing updating stack
	// This is the reason why the states can REQUEST a PendingChange struct to push/pop on the StateStack
	// Then, the stateStack applies the pending changes all at once at a safe point in time
	void					applyPendingChanges();

private:
	// Contains all the requested changes and 
	struct PendingChange
	{
		explicit			PendingChange(Action action, States::ID stateID = States::None);

		Action				action;
		States::ID			stateID;
	};
private:
	std::vector<State::Ptr>			mStack;
	std::vector<PendingChange>		mPendingList;
	State::Context					mContext; 
	// Map ID's to functions that return a new State object
	std::map<States::ID, std::function<State::Ptr()> > mFactories;

};

// Creates the functions for the factory(which is a map of functions)
// accepts a TEMPLATE for the polymorphic types
template <typename T>
void StateStack::registerState(States::ID stateID)
{
	// 'this' capture specifier
	mFactories[stateID] = [this]()
	{
		// Creates a new stack with a context of the previous sttack
		// Creates a new stack with type T(since we wont know exact type since we're polymorphic)
		// If we make a new state on the stack, we want to register it with a specific ID
		// and not only that, we want to make a factory for the registered stacks
		return State::Ptr(new T(*this, mContext)); // needs the 'this' pointer to get the StateStack
	};
}

