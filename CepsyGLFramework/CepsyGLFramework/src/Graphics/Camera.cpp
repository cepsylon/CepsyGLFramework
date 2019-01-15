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
	// Camera's rotation inverse, compute quaternion inverse first
	return glm::mat4_cast(glm::inverse(owner().transform().rotation()));
}
