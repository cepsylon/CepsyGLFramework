#include "Entity.h"

#include "Component.h"

void Entity::initialize()
{
	// Initialize all component
	for (auto & component : mComponents)
		component->initialize();
}

void Entity::update()
{
	// Update all components
	for (auto & component : mComponents)
		component->update();
}

void Entity::shutdown()
{
	// Shutdown all components
	for (auto & component : mComponents)
		component->shutdown();
}
