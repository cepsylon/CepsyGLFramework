#pragma once

#include "Common/Base.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
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

		glm::mat4 mBindMatrix;
		std::vector<unsigned> mChildrenIndices;
		std::string mName;
		glm::quat mRotation;
		glm::vec3 mPosition;
	};

	Skeleton() = default;
	Skeleton(std::vector<Bone> && bones);

	// Check if skeleton is empty
	bool empty() const;

	// Debug draw
	void debug_draw(const glm::mat4 & model_matrix) const;

	// Find index of the given name for the bone
	int find(const std::string & bone_name) const;

private:
	void debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const;

	std::vector<Bone> mBones;
};
