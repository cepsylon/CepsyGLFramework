#pragma once

#include "Common/Base.h"
#include "Common/Transform.h"

#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

class Skeleton : public Base
{
public:
	RTTI_H;

	struct Bone
	{
		glm::mat4 matrix() const;

		std::vector<unsigned> mChildrenIndices;
		std::string mName;
		glm::quat mRotation;
		glm::vec3 mPosition;
	};

	Skeleton() = default;
	Skeleton(std::vector<Bone> && bones);

	bool empty() const;

	void debug_draw(const glm::mat4 & model_matrix) const;

private:
	void debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const;

	std::vector<Bone> mBones;
};
