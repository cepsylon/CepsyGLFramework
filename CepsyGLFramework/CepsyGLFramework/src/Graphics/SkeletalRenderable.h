#pragma once

#include "Renderable.h"
#include "AnimationData.h"

#include <glm/mat4x4.hpp>

#include <vector>

class Animation;

class SkeletalRenderable : public Renderable
{
public:
	RTTI_H;

	// Update animation if any
	void update(float dt) override;
	// GUI
	void to_gui() override;

	// Binds needed data for rendering
	void bind() const override;

	// Set animation
	void set_animation(const std::string & name);

	// Get animation data
	AnimationData & animation_data();

private:
	AnimationData mAnimationData;
};
