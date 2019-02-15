#include "Animation.h"

RTTI_I(Animation, Base);

Animation::Animation(int bone_count)
	: mKeyframes(bone_count)
{ }

Animation::Animation(Animation && rhs)
	: mKeyframes(std::move(rhs.mKeyframes))
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

float Animation::duration() const { return mKeyframes.front().mTranslation.back().mTime; }
