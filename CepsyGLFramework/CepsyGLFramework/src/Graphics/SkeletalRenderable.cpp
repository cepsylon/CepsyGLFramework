#include "SkeletalRenderable.h"

#include "Animation.h"
#include "Skeleton.h"
#include "Application/Application.h"

#include <imgui/imgui.h>

void SkeletalRenderable::AnimationData::update()
{
	if (mAnimation)
	{
		mTime += ImGui::GetIO().DeltaTime;

		while (mAnimation->duration() < mTime)
		{
			mTime -= mAnimation->duration();
			mPrevIndex = mIndex;
			mIndex = 0;
		}

		while (mTime > (*mAnimation)[0].mRotation[mIndex].mTime)
		{
			mPrevIndex = mIndex;
			mIndex++;
		}
	}
}

void SkeletalRenderable::AnimationData::upload() const
{
	if (mAnimation)
	{
		std::vector<glm::mat4> matrices = get_matrices();
		application.graphics().skeleton_buffer().update(matrices.data(), matrices.size() * sizeof(glm::mat4));
	}
}

std::vector<glm::mat4> SkeletalRenderable::AnimationData::get_matrices() const
{
	float t = mIndex == 0u ? mTime / (*mAnimation)[0].mRotation[mIndex].mTime :
		(mTime - (*mAnimation)[0].mRotation[mPrevIndex].mTime) / ((*mAnimation)[0].mRotation[mIndex].mTime - (*mAnimation)[0].mRotation[mPrevIndex].mTime);
	return mAnimation->get_matrices_at(t, mPrevIndex, mIndex);
}

void SkeletalRenderable::update()
{
	mAnimationData.update();
}

void SkeletalRenderable::bind() const
{
	mAnimationData.upload();
}

void SkeletalRenderable::set_animation(const std::string & name)
{
	mAnimationData.mAnimation = application.resources().get<Animation>(name);
}

SkeletalRenderable::AnimationData & SkeletalRenderable::animation_data() { return mAnimationData; }
