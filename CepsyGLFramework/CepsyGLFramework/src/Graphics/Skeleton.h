#pragma once

#include "Common/Base.h"
#include "Common/Transform.h"

#include <vector>

class Skeleton : public Base
{
public:
	RTTI_H;

	struct Bone
	{
		glm::vec3 mPosition;
		glm::vec3 mRotation;
		std::vector<unsigned> mChildrenIndices;
	};

	Skeleton() = default;
	Skeleton(std::vector<Bone> && bones);

private:
	std::vector<Bone> mBones;
};
