#include "Animation.h"

RTTI_I(Animation, Base);

Animation::Animation(int bone_count)
	: mKeyframes(bone_count)
{ }

Animation::Animation(Animation && rhs)
	: mKeyframes(std::move(rhs.mKeyframes))
{ }

std::vector<Animation::Keyframe> & Animation::operator[](unsigned index)
{ 
	if (mKeyframes.size() <= index)
		throw "Index out of range\n";
	return mKeyframes[index];
}
