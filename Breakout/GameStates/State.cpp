#include "State.h"
#include "StateStack.h"


State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts)
	: window(&window)
	, textures(&textures)
	, fonts(&fonts)
{ }

State::State(StateStack& stack, Context context)
	: mStack(&stack)
	, mContext(context)
{ }

State::~State() { }

void State::requestStackPush(States::ID stateID)
{
	// Doesn't ACTUALLY push a state, it REQUESTS one
	// mStack->pushState just pushes a PendingChange
	mStack->requestStatePush(stateID);
}

void State::requestStackPop()
{
	mStack->requestStatePop();
}

void State::requestStateClear()
{
	mStack->requestStatesClear();
}

State::Context State::getContext() const
{
	return mContext;
}