#include "Entity.h"

#include "Component.h"

#include <imgui/imgui.h>

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

void Entity::to_gui()
{
	// Transform to gui
	if (ImGui::TreeNode("Transform"))
	{
		mTransform.to_gui();
		ImGui::TreePop();
	}

	// Components to gui
	for (auto & component : mComponents)
	{
		if (ImGui::TreeNode(component->class_name()))
		{
			component->to_gui();
			ImGui::TreePop();
		}
	}
}

const Transform & Entity::transform() const { return mTransform; }
Transform & Entity::transform() { return mTransform; }
