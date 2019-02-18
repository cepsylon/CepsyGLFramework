#pragma once

#include "Base.h"

class Entity;

class Component : public Base
{
public:
	RTTI_H;

	virtual ~Component() {}

	// Initialize
	virtual void initialize() = 0;

	// Update
	virtual void update(float dt) = 0;

	// Shutdown
	virtual void shutdown() = 0;

	// Outputs to gui
	virtual void to_gui() = 0;

	// Gets owner
	Entity & owner();
	const Entity & owner() const;

	// Sets owner
	void set_owner(Entity * owner);
	void set_owner(Entity & owner);

private:
	Entity * mOwner = nullptr;
};
