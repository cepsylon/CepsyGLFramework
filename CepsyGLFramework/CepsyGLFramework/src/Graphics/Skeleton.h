#pragma once

#include "Common/Base.h"
#include "Common/Transform.h"

#include <vector>
#include <string>

class Skeleton : public Base
{
public:
	RTTI_H;

	struct Bone
	{
		std::vector<unsigned> mChildrenIndices;
		std::string mName;
		glm::vec3 mPosition;
		glm::vec3 mRotation;
	};

	Skeleton() = default;
	Skeleton(std::vector<Bone> && bones);

private:
	std::vector<Bone> mBones;
};
