#pragma once

#include "Common/Base.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include <vector>

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

	Animation(int bone_count);
	Animation(Animation && rhs);

	Keyframe & operator[](unsigned index);
	const Keyframe & operator[](unsigned index) const;

	float duration() const;

private:
	std::vector<Keyframe> mKeyframes;
};
