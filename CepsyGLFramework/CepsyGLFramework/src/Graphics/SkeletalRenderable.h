#pragma once

#include "Renderable.h"

#include <glm/mat4x4.hpp>

#include <vector>

class Animation;

class SkeletalRenderable : public Renderable
{
public:

	struct AnimationData
	{
		void update();
		void upload() const;
		std::vector<glm::mat4> get_matrices() const;

		const Animation * mAnimation = nullptr;
		float mTime = 0.0f;
		unsigned mIndex = 1u;
		unsigned mPrevIndex = 0u;
	};

	// Update animation if any
	void update() override;

	// Binds needed data for rendering
	void bind() const override;

	// Set animation
	void set_animation(const std::string & name);

	// Get animation data
	AnimationData & animation_data();

private:
	AnimationData mAnimationData;
};
