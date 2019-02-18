#include "AnimationData.h"

#include "Animation.h"
#include "Application/Application.h"

void AnimationData::update(float dt)
{
	if (mAnimation)
	{
		mTime += dt;

		while (mAnimation->duration() < mTime)
		{
			mTime -= mAnimation->duration();
			mPrevIndex = mIndex;
			mIndex = 0;
		}

		// TODO: fix this, we are assuming we rotate the root, it may not be the case
		while (mTime > (*mAnimation)[0].mRotation[mIndex].mTime)
		{
			mPrevIndex = mIndex;
			mIndex++;
		}
	}
}

void AnimationData::upload_to_gpu() const
{
	if (mAnimation)
	{
		std::vector<glm::mat4> matrices = get_matrices();
		application.graphics().skeleton_buffer().update(matrices.data(), matrices.size() * sizeof(glm::mat4));
	}
}

void AnimationData::set_animation(const std::string & name)
{
	mAnimation = application.resources().get<Animation>(name);
}

std::vector<glm::mat4> AnimationData::get_matrices() const
{
	// TODO: fix this, we are assuming we rotate the root, it may not be the case
	float t = mIndex == 0u ? mTime / (*mAnimation)[0].mRotation[mIndex].mTime :
		(mTime - (*mAnimation)[0].mRotation[mPrevIndex].mTime) / ((*mAnimation)[0].mRotation[mIndex].mTime - (*mAnimation)[0].mRotation[mPrevIndex].mTime);
	return mAnimation->get_matrices_at(t, mPrevIndex, mIndex);
}
