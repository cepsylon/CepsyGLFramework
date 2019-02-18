#include "SkeletalRenderable.h"

RTTI_I(SkeletalRenderable, Renderable);

void SkeletalRenderable::update(float dt)
{
	mAnimationData.update(dt);
}

void SkeletalRenderable::to_gui()
{
	Renderable::to_gui();
	mAnimationData.to_gui();
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
