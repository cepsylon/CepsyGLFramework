#include "SkeletalModel.h"

SkeletalModel::SkeletalModel(std::vector<Mesh> && meshes, std::vector<Material> && materials, Skeleton && skeleton)
	: Model(std::move(meshes), std::move(materials))
	, mSkeleton(std::move(skeleton))
{ }

SkeletalModel::SkeletalModel(SkeletalModel && rhs)
	: Model(std::move(rhs))
	, mSkeleton(std::move(rhs.mSkeleton))
{ }

SkeletalModel & SkeletalModel::operator=(SkeletalModel && rhs)
{
	if (this != &rhs)
	{
		Model::operator=(std::move(rhs));
		mSkeleton = std::move(rhs.mSkeleton);
	}

	return *this;
}

void SkeletalModel::draw(const Program * program) const
{
	// Set skeleton

	// Draw call
	//Model::draw(program);
}

const Skeleton & SkeletalModel::skeleton() const { return mSkeleton; }
