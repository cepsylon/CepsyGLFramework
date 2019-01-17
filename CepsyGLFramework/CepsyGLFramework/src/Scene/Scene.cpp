#include "Scene.h"

#include <imgui/imgui.h>

// This will be removed soon
#include "Graphics/Camera.h"

void Scene::initialize()
{
	//TODO actually loading things from file

	// For now
	// Create an empty space
	mSpaces.emplace_back(std::make_unique<Space>());
	mSpaces.back()->set_name("Space");

	// Add camera to space
	std::unique_ptr<Entity> camera = std::make_unique<Entity>();
	camera->set_name("Camera");
	camera->transform().set_position(glm::vec3{ 0.0f, 0.0f, 5.0f });
	camera->add<Camera>();
	mSpaces.back()->add(std::move(camera));
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

void Scene::to_gui()
{
	ImGui::Begin("Scene");
	ImGui::PushItemWidth(150.0f);
	for (auto & space : mSpaces)
	{
		if (ImGui::TreeNode(space->name().c_str()))
		{
			space->to_gui();
			ImGui::TreePop();
		}
	}
	ImGui::PopItemWidth();
	ImGui::End();
}
