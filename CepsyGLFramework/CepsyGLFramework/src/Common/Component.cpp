#include "Component.h"

Entity & Component::owner()
{
	return *mOwner;
}

const Entity & Component::owner() const
{
	return *mOwner;
}

void Component::set_owner(Entity * owner)
{
	mOwner = owner;
}

void Component::set_owner(Entity & owner)
{
	mOwner = &owner;
}
