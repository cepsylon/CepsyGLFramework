#include "Space.h"

#include <imgui/imgui.h>

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

void Space::to_gui()
{
	for (auto & entity : mEntities)
	{
		if (ImGui::TreeNode(entity->name().c_str()))
		{
			entity->to_gui();
			ImGui::TreePop();
		}
	}
}

void Space::add(std::unique_ptr<Entity> && entity)
{
	mEntities.emplace_back(std::move(entity));
}
