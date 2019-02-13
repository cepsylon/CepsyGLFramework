#pragma once

#include "Common/Base.h"
#include "Buffer.h"

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

	// No copy
	Skeleton(const Skeleton &) = delete;
	Skeleton & operator=(const Skeleton &) = delete;

	// Move only
	Skeleton(Skeleton && rhs);
	Skeleton & operator=(Skeleton && rhs);

	// Check if skeleton is empty
	bool empty() const;

	// Update skeleton matrix buffer
	void update();

	// Bind skeleton matrix buffer
	void bind() const;

	// Debug draw
	void debug_draw(const glm::mat4 & model_matrix) const;

	// Find index of the given name for the bone
	int find(const std::string & bone_name) const;

	// Get bone matrices
	std::vector<glm::mat4> get_bone_matrices() const;

private:
	// Assumes the index has a parent index
	void get_bone_matrices(std::vector<glm::mat4> & matrices, int index, const glm::mat4 & parent_matrix) const;

	// Debug draw for the skeleton, simple lines from link to link
	void debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const;

	std::vector<Bone> mBones;
	BufferF32 mBuffer{GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW, GL_FLOAT };
};
