#include "EditorCamera.h"

#include "Common/Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <imgui/imgui.h>

void EditorCamera::initialize()
{
	Camera::initialize();

	glm::vec3 angles = owner().transform().euler_angles();
	mRotation.x = angles.x;
	mRotation.y = angles.y;
}

void EditorCamera::update(float dt)
{
	Camera::update(dt);

	// Don't mess while editing GUI
	if (ImGui::IsAnyItemActive())
		return;

	// Compute axes
	if (ImGui::IsKeyDown('J'))
		mRotation.y += dt;
	if (ImGui::IsKeyDown('L'))
		mRotation.y -= dt;
	if (ImGui::IsKeyDown('I'))
		mRotation.x += dt;
	if (ImGui::IsKeyDown('K'))
		mRotation.x -= dt;

	glm::vec3 right{ std::cos(mRotation.y), 0.0f, -std::sin(mRotation.y) };
	glm::vec4 helper = glm::rotate(glm::mat4{ 1.0f }, mRotation.x, right) * glm::vec4{ 0.0f, 1.0f, 0.0f, 0.0f };
	glm::vec3 up{ helper.x, helper.y, helper.z };
	glm::vec3 forward = glm::cross(right, up);

	// Set orientation
	owner().transform().set_rotation(glm::quat_cast(glm::mat3{ right, up, forward }));

	// Movement
	glm::vec3 movement{ 0.0f };
	if (ImGui::IsKeyDown('A'))
		movement.x -= dt;
	if (ImGui::IsKeyDown('D'))
		movement.x += dt;
	if (ImGui::IsKeyDown('S'))
		movement.y -= dt;
	if (ImGui::IsKeyDown('W'))
		movement.y += dt;
	if (ImGui::IsKeyDown('Z'))
		movement.z -= dt;
	if (ImGui::IsKeyDown('X'))
		movement.z += dt;

	// Compute new position
	glm::vec3 position = owner().transform().position();
	owner().transform().set_position(position + movement.x * right + movement.y * up + movement.z * forward);
}
