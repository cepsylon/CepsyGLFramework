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
	std::vector<unsigned> mChildrenIndices;
	std::string mName;
	glm::quat mRotation;
	glm::vec3 mPosition;
	int mParent = -1;
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

	// Find index of the given name for the bone
	int find(const std::string & bone_name) const;

	// Check if skeleton is empty
	bool empty() const;

	// Get skin matrices
	const std::vector<glm::mat4> & skin_matrices() const;

	// Get bone count
	unsigned bone_count() const;

	// Get root
	const Bone & root() const;

	// Get bone at index
	const Bone & bone(unsigned i) const;

private:
	std::vector<Bone> mBones;
	std::vector<glm::mat4> mSkinMatrices;
};
