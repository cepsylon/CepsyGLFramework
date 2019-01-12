#include "Space.h"

#include "Common/Entity.h"
#include "Common/Component.h"

void Space::initialize()
{
	for (auto & entity : mEntities)
		entity->initialize();
}

void Space::update()
{
	for (auto & entity : mEntities)
		entity->update();
}

void Space::shutdown()
{
	for (auto & entity : mEntities)
		entity->shutdown();
}

void Space::add(std::unique_ptr<Entity> && entity)
{
	mEntities.emplace_back(std::move(entity));
}
