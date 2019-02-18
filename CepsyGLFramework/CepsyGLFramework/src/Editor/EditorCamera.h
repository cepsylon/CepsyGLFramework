#pragma once

#include "Graphics/Camera.h"

#include <glm/vec2.hpp>

class EditorCamera : public Camera
{
public:
	void initialize() override;
	void update(float dt) override;

private:
	glm::vec2 mRotation{ 0.0f };
};
