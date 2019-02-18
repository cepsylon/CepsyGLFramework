#include "SkeletalRenderable.h"

void SkeletalRenderable::update(float dt)
{
	mAnimationData.update(dt);
}

void SkeletalRenderable::bind() const
{
	mAnimationData.upload_to_gpu();
}

void SkeletalRenderable::set_animation(const std::string & name)
{
	mAnimationData.set_animation(name);
}

AnimationData & SkeletalRenderable::animation_data() { return mAnimationData; }
