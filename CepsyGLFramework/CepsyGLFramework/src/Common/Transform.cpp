#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

#include <imgui/imgui.h>

Transform::Transform(const Transform & rhs)
	: mRotation(rhs.mRotation)
	, mTranslation(rhs.mTranslation)
	, mScale(rhs.mScale)
{ }

void Transform::to_gui()
{
	ImGui::InputFloat3("Position", &mTranslation[0]);
	ImGui::InputFloat3("Scale", &mScale[0]);
	glm::vec3 degrees = glm::eulerAngles(mRotation);
	if (ImGui::InputFloat3("Rotation", &degrees[0]))
		mRotation = glm::quat{ degrees };
}

glm::quat Transform::rotation() const {	return mRotation; }
glm::vec3 Transform::position() const { return mTranslation; }
glm::vec3 Transform::scale() const { return mScale; }
glm::vec3 Transform::euler_angles() const { return glm::eulerAngles(mRotation); }

void Transform::set_rotation(const glm::vec3 & rotation) { mRotation = glm::quat{ rotation }; }
void Transform::set_rotation(const glm::quat & rotation) { mRotation = rotation; }
void Transform::set_position(const glm::vec3 & position) { mTranslation = position; }
void Transform::set_scale(const glm::vec3 & scale) { mScale = scale; }

glm::mat4 Transform::model() const
{
	glm::mat4 model = glm::mat4_cast(mRotation);
	model[0] *= mScale.x;
	model[1] *= mScale.y;
	model[2] *= mScale.z;
	model[3][0] = mTranslation.x;
	model[3][1] = mTranslation.y;
	model[3][2] = mTranslation.z;
	return model;
}
