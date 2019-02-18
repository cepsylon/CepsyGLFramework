#include "SkeletalRenderable.h"

#include <imgui/imgui.h>

void SkeletalRenderable::update()
{
	mAnimationData.update(ImGui::GetIO().DeltaTime);
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
