#pragma once

#include "Common/Base.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <vector>

class Skeleton;

class Animation : public Base
{
public:
	RTTI_H;

	struct KeyframeRotation
	{
		glm::quat mRotation;
		float mTime;
	};

	struct KeyframeTranslation
	{
		glm::vec3 mTranslation;
		float mTime;
	};

	struct Keyframe
	{
		std::vector<KeyframeRotation> mRotation;
		std::vector<KeyframeTranslation> mTranslation;
	};

	Animation(const Skeleton & skeleton);
	Animation(Animation && rhs);

	// Get keyframe at index
	Keyframe & operator[](unsigned index);
	const Keyframe & operator[](unsigned index) const;

	// Get current skinning matrices
	std::vector<glm::mat4> get_matrices_at(float t, unsigned prev_index, unsigned index) const;

	// Get skeleton
	const Skeleton & skeleton() const;

	// Animation duration
	float duration() const;

private:
	std::vector<Keyframe> mKeyframes;
	const Skeleton & mSkeleton;
};
