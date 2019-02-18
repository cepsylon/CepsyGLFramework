#pragma once

#include "Resources/ResourceHandle.h"

#include <glm/mat4x4.hpp>

#include <vector>
#include <string>

class Animation;

class AnimationData
{
public:
	// Update animation
	void update(float dt);

	// Upload data to GPU
	void upload_to_gpu() const;

	// Set animation
	void set_animation(const std::string & name);

private:
	// Get skinning matrices
	std::vector<glm::mat4> get_matrices() const;

	ResourceHandle<Animation> mAnimation;
	float mTime = 0.0f;
	unsigned mIndex = 1u;
	unsigned mPrevIndex = 0u;
};
