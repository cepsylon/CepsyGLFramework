#include "Skeleton.h"

RTTI_I(Skeleton, Base);

Skeleton::Skeleton(std::vector<Bone> && bones)
	: mBones(std::move(bones))
{ }
