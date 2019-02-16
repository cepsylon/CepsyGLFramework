#pragma once

#include "Common/Base.h"
#include "Buffer.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

struct Bone
{
	glm::mat4 matrix() const;

	std::vector<unsigned> mChildrenIndices;
	std::string mName;
	glm::quat mRotation;
	glm::vec3 mPosition;
};

class Skeleton : public Base
{
public:
	RTTI_H;

	friend class SkeletonImporter;

	Skeleton() = default;

	// No copy
	Skeleton(const Skeleton &) = default;
	Skeleton & operator=(const Skeleton &) = default;

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

	// Get bind matrices
	const std::vector<glm::mat4> & skin_matrices() const;

	// Get bone count
	int bone_count() const;

	// Get root
	const Bone & root() const;

private:
	// Assumes the index has a parent index
	void get_bone_matrices(std::vector<glm::mat4> & matrices, int index, const glm::mat4 & parent_matrix) const;

	// Debug draw for the skeleton, simple lines from link to link
	void debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const;

	std::vector<Bone> mBones;
	std::vector<glm::mat4> mSkinMatrices;
};
