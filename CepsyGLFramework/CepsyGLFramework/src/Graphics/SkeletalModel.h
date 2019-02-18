#pragma once

#include "Model.h"
#include "Resources/ResourceHandle.h"

class Skeleton;

class SkeletalModel : public Model
{
public:
	RTTI_H;

	SkeletalModel(std::vector<Mesh> && meshes, std::vector<Material> && materials, ResourceHandle<Skeleton> skeleton);

	// No copies
	SkeletalModel(const SkeletalModel &) = delete;
	SkeletalModel & operator=(const SkeletalModel &) = delete;

	// Only moves
	SkeletalModel(SkeletalModel && rhs);
	SkeletalModel & operator=(SkeletalModel && rhs);

	void draw(const Program * program) const override;

	// Gettors
	const Skeleton & skeleton() const;

private:
	 ResourceHandle<Skeleton> mSkeleton;
};
