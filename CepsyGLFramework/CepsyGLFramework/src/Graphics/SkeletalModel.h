#pragma once

#include "Model.h"

class Skeleton;

class SkeletalModel : public Model
{
public:
	RTTI_H;

	SkeletalModel(std::vector<Mesh> && meshes, std::vector<Material> && materials, const Skeleton * skeleton);

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
	 const Skeleton * mSkeleton = nullptr;
};
