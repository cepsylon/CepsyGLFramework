#include "SkeletalModel.h"

#include "Skeleton.h"

RTTI_I(SkeletalModel, Model);

SkeletalModel::SkeletalModel(std::vector<Mesh> && meshes, std::vector<Material> && materials, Skeleton * skeleton)
	: Model(std::move(meshes), std::move(materials))
	, mSkeleton(skeleton)
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
	(*mSkeleton).bind();
	Model::draw(program);
}

const Skeleton & SkeletalModel::skeleton() const { return *mSkeleton; }
Skeleton & SkeletalModel::skeleton() { return *mSkeleton; }
