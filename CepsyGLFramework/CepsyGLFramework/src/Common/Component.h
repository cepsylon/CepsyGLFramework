#pragma once

#include "Base.h"

class Entity;

class Component : public Base
{
public:
	// Initialize
	virtual void initialize() = 0;

	// Update
	virtual void update() = 0;

	// Shutdown
	virtual void shutdown() = 0;

	// Gets owner
	Entity & owner();
	const Entity & owner() const;

	// Sets owner
	void set_owner(Entity * owner);
	void set_owner(Entity & owner);

private:
	Entity * mOwner = nullptr;
};