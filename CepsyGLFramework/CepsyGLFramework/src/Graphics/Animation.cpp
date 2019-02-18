#include "Animation.h"

#include "Skeleton.h"

RTTI_I(Animation, Base);

Animation::Animation(const Skeleton & skeleton)
	: mKeyframes(skeleton.bone_count())
	, mSkeleton(skeleton)
{ }

Animation::Animation(Animation && rhs)
	: mKeyframes(std::move(rhs.mKeyframes))
	, mSkeleton(rhs.mSkeleton)
{ }

Animation::Keyframe & Animation::operator[](unsigned index)
{ 
	if (mKeyframes.size() <= index)
		throw "Index out of range\n";
	return mKeyframes[index];
}

const Animation::Keyframe & Animation::operator[](unsigned index) const
{
	if (mKeyframes.size() <= index)
		throw "Index out of range\n";
	return mKeyframes[index];
}

std::vector<glm::mat4> Animation::get_matrices_at(float t, unsigned prev_index, unsigned index) const
{
	std::vector<glm::mat4> matrices(mSkeleton.bone_count(), glm::mat4{ 1.0f });

	for (unsigned i = 0; i < mSkeleton.bone_count(); ++i)
	{
		const Bone & bone = mSkeleton.bone(i);
		bool rot = mKeyframes[i].mRotation.empty() == false;
		bool pos = mKeyframes[i].mTranslation.empty() == false;
		if (rot == false && pos == false)
		{
			if (bone.mParent != -1)
				matrices[i] = matrices[bone.mParent];
			continue;
		}

		glm::vec3 position = bone.mPosition;
		glm::quat rotation = bone.mRotation;
		if (pos)
			position = glm::mix(mKeyframes[i].mTranslation[prev_index].mTranslation, mKeyframes[i].mTranslation[index].mTranslation, t);
		if (rot)
			rotation = glm::slerp(mKeyframes[i].mRotation[prev_index].mRotation, mKeyframes[i].mRotation[index].mRotation, t);

		matrices[i] = glm::mat4_cast(rotation);
		matrices[i][3][0] = position.x;
		matrices[i][3][1] = position.y;
		matrices[i][3][2] = position.z;
		if (bone.mParent != -1)
			matrices[i] = matrices[bone.mParent] * matrices[i];
	}

	const std::vector<glm::mat4> & skin_matrices = mSkeleton.skin_matrices();
	for (unsigned i = 0; i < matrices.size(); ++i)
		matrices[i] = matrices[i] * skin_matrices[i];

	return matrices;
}

const Skeleton & Animation::skeleton() const { return mSkeleton; }
float Animation::duration() const { return mKeyframes.front().mTranslation.back().mTime; }
