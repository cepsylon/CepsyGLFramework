#pragma once

#include <vector>
#include <memory>

class Entity;

class Space
{
public:
	// Initialize all entities
	void initialize();
	// Update all entities
	void update();
	// Shutdown all entities
	void shutdown();

	// Add already built entity
	void add(std::unique_ptr<Entity> && entity);
private:
	std::vector<std::unique_ptr<Entity>> mEntities;
};
