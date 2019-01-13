#include "Scene.h"

void Scene::initialize()
{
	//TODO actually loading things from file

	// For now
	// Create an empty space
	mSpaces.emplace_back(std::make_unique<Space>());
}

void Scene::update()
{
	// Update spaces
	for (auto & space : mSpaces)
		space->update();
}

void Scene::shutdown()
{
	for (auto & space : mSpaces)
		space->shutdown();

	// TODO Store the scene to file
}
