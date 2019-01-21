#include "Camera.h"

#include "Application/Application.h"
#include "Common/Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

RTTI_I(Camera, Component);

Camera::Camera()
	: mFieldOfViewY(glm::radians(60.0f))
	, mRatio(mViewport.ratio())
	, mFar(1000.0f)
	, mNear(0.1f)
{
	application.graphics().add(this);
}

Camera::~Camera()
{
	application.graphics().remove(this);
}

void Camera::initialize()
{

}

void Camera::update()
{

}

void Camera::shutdown()
{

}

void Camera::to_gui()
{
	if (ImGui::TreeNode("Viewport"))
	{
		mViewport.to_gui();
		ImGui::TreePop();
	}
	ImGui::Combo("Projection", &mProjectionMode, "Perspective\0Orthogonal\0");
	if (mProjectionMode == Perspective)
	{
		ImGui::SliderAngle("Field Of View Y", &mFieldOfViewY, 0.0f, 90.0f);
		ImGui::InputFloat("Near", &mNear);
		ImGui::InputFloat("Far", &mFar);
	}
	else
	{
		ImGui::InputFloat("Left", &mLeft);
		ImGui::InputFloat("Right", &mRight);
		ImGui::InputFloat("Bottom", &mBottom);
		ImGui::InputFloat("Top", &mTop);
	}
}

void Camera::set() const
{
	// Set viewport
	mViewport.set();

	// Pass camera matrices to GPU
	application.graphics().update_camera_matrices(projection(), view());
}

glm::mat4 Camera::projection() const
{
	if (mProjectionMode == Perspective)
		return glm::perspective(mFieldOfViewY, mRatio, mNear, mFar);
	else
		return glm::ortho(mLeft, mRight, mBottom, mTop);
}

glm::mat4 Camera::view() const
{
	// Inverse of the rotation
	glm::quat rotation = owner().transform().rotation();
	float divisor = glm::dot(rotation, rotation);
	if (divisor != 0.0f)
		rotation = glm::conjugate(rotation) / divisor;

	// Compute view matrix
	glm::mat4 view = glm::mat4_cast(rotation);
	glm::vec3 eye = owner().transform().position();

	// Apply camera position inverse
	view[3][0] = -glm::dot(glm::vec3{ view[0][0], view[1][0], view[2][0] }, eye);
	view[3][1] = -glm::dot(glm::vec3{ view[0][1], view[1][1], view[2][1] }, eye);
	view[3][2] = -glm::dot(glm::vec3{ view[0][2], view[1][2], view[2][2] }, eye);
	return view;
}
