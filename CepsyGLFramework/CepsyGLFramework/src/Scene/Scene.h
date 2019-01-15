#pragma once

#include "Space.h"

#include <vector>
#include <memory>

class Scene
{
public:
	// Initialized scene, loads the previously saved scene
	void initialize();
	// Update all scene entities
	void update();
	// Saves the current scene
	void shutdown();

	// Outputs scene to gui
	void to_gui();

private:
	std::vector<std::unique_ptr<Space>> mSpaces;
};
