#include "Skeleton.h"

#include "Animation.h"
#include "Application/Application.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>

RTTI_I(Skeleton, Base);

Skeleton::Skeleton(Skeleton && rhs)
	: mBones(std::move(rhs.mBones))
	, mSkinMatrices(std::move(rhs.mSkinMatrices))
{ }

Skeleton & Skeleton::operator=(Skeleton && rhs)
{
	if (this != &rhs)
	{
		mBones = std::move(rhs.mBones);
		mSkinMatrices = std::move(rhs.mSkinMatrices);
	}

	return *this;
}

int Skeleton::find(const std::string & bone_name) const
{
	for (unsigned i = 0; i < mBones.size(); ++i)
	{
		if (mBones[i].mName == bone_name)
			return static_cast<int>(i);
	}

	return -1;
}


bool Skeleton::empty() const { return mBones.empty(); }
const std::vector<glm::mat4> & Skeleton::skin_matrices() const { return mSkinMatrices; }
unsigned Skeleton::bone_count() const { return mBones.size(); }
const Bone & Skeleton::root() const { return mBones.front(); }
const Bone & Skeleton::bone(unsigned i) const
{
	if (mBones.size() <= i)
		throw "Index out of range\n";
	return mBones[i];
}
