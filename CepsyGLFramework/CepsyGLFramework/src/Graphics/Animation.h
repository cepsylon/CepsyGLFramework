#pragma once

#include "Common/Base.h"

#include <glm/gtc/quaternion.hpp>

#include <vector>

class Animation : public Base
{
public:
	RTTI_H;

	struct Keyframe
	{
		glm::quat mRotation;
		float mTime;
	};

	Animation(int bone_count);
	Animation(Animation && rhs);

	std::vector<Keyframe> & operator[](unsigned index);

private:
	std::vector<std::vector<Keyframe>> mKeyframes;
};
