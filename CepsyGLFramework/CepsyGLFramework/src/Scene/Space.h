#pragma once

#include "Common/Base.h"
#include "Common/Entity.h"

#include <vector>
#include <memory>

class Space : public Base
{
public:
	// Initialize all entities
	void initialize();
	// Update all entities
	void update();
	// Shutdown all entities
	void shutdown();

	// Outputs to gui
	void to_gui();

	// Add already built entity
	void add(std::unique_ptr<Entity> && entity);
private:
	std::vector<std::unique_ptr<Entity>> mEntities;
};
