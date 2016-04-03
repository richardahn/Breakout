#include "StateStack.h"

#include <cassert>


StateStack::StateStack(State::Context context)
	: mStack()
	, mPendingList()
	, mContext(context)
	, mFactories()
{}

void StateStack::update(sf::Time dt)
{
	// Iterate from top to bottom, STOP updating when a state returns false 
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->update(dt))
			break;
	}

	// Apply all the pending stack operations after updating
	applyPendingChanges();
}

void StateStack::draw()
{
	// Draw all active states from bottom to top
	for (State::Ptr& state : mStack)
		state->draw();
}

void StateStack::handleEvent(const sf::Event& event)
{
	// Iterate from top to bottom, stop as soon as handleEvent() returns false
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->handleEvent(event))
			break;
	}

	applyPendingChanges();
}

void StateStack::requestStatePush(States::ID stateID)
{
	mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::requestStatePop()
{
	mPendingList.push_back(PendingChange(Pop));
}

void StateStack::requestStatesClear()
{
	mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
	return mStack.empty();
}

// Make sure that you've registered the state before creating it, otherwise assert error
State::Ptr StateStack::createState(States::ID stateID)
{
	// A Map returns the key/value pair in std::pair, so you call the function which is the second
	// part of the pair, which returns a Ptr to the newly created state
	auto found = mFactories.find(stateID);
	assert(found != mFactories.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Push:
			mStack.push_back(createState(change.stateID));
			break;

		case Pop:
			mStack.pop_back();
			break;

		case Clear:
			mStack.clear();
			break;
		}
	}

	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(StateStack::Action action, States::ID stateID)
	: action(action)
	, stateID(stateID)
{
}