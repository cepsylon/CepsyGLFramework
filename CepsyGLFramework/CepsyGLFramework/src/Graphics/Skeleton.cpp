#include "Skeleton.h"

#include "Animation.h"
#include "Application/Application.h"
#include "imgui/imgui.h"

#include <glm/glm.hpp>

RTTI_I(Skeleton, Base);

glm::mat4 Bone::matrix() const
{
	glm::mat4 matrix = glm::mat4_cast(mRotation);
	matrix[3][0] = mPosition.x;
	matrix[3][1] = mPosition.y;
	matrix[3][2] = mPosition.z;
	return matrix;
}

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

bool Skeleton::empty() const { return mBones.empty(); }

void Skeleton::update()
{
	static float time = 0.0f;
	static int prev_index = 0;
	static int index = 1;
	const Animation & animation = *application.resources().get<Animation>("foot_idle_no_skin");

	time += ImGui::GetIO().DeltaTime / 2.0f;

	if (animation.duration() < time)
	{
		time -= animation.duration();
		prev_index = index;
		index = 0;
	}

	while (time > animation[0].mRotation[index].mTime)
	{
		prev_index = index;
		index++;
	}

	for (unsigned i = 0; i < mBones.size(); ++i)
	{
		bool rot = animation[i].mRotation.empty() == false;
		bool pos = animation[i].mTranslation.empty() == false;
		if (rot == false && pos == false)
			continue;

		float t = index == 0u ? time / animation[i].mRotation[index].mTime :
			(time - animation[i].mRotation[prev_index].mTime) / (animation[i].mRotation[index].mTime - animation[i].mRotation[prev_index].mTime);
		if(pos)
			mBones[i].mPosition = glm::mix(animation[i].mTranslation[prev_index].mTranslation, animation[i].mTranslation[index].mTranslation, t);
		if (rot)
			mBones[i].mRotation = glm::slerp(animation[i].mRotation[prev_index].mRotation, animation[i].mRotation[index].mRotation, t);
	}

}

void Skeleton::bind() const
{
	// Update buffer
	std::vector<glm::mat4> bone_matrices = get_bone_matrices();
	application.graphics().skeleton_buffer().update(bone_matrices.data(), bone_matrices.size() * sizeof(glm::mat4));
}

void Skeleton::debug_draw(const glm::mat4 & model_matrix) const
{
	glm::vec4 bone_position = model_matrix * glm::vec4{ mBones.front().mPosition, 1.0f };
	glm::mat4 parent_matrix = model_matrix * mBones.front().matrix();
	for (const auto & child_index : mBones.front().mChildrenIndices)
		debug_draw_rec(parent_matrix, bone_position, child_index);
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

std::vector<glm::mat4> Skeleton::get_bone_matrices() const
{
	if (mBones.empty())
		return std::vector<glm::mat4>{};

	// Parent matrix
	std::vector<glm::mat4> bone_matrices(mBones.size());
	glm::mat4 parent_matrix = mBones.front().matrix();
	bone_matrices.front() = parent_matrix * mSkinMatrices.front();

	// Call recursion on children
	for (const int & child_index : mBones.front().mChildrenIndices)
		get_bone_matrices(bone_matrices, child_index, parent_matrix);

	return bone_matrices;
}

void Skeleton::get_bone_matrices(std::vector<glm::mat4> & matrices, int index, const glm::mat4 & parent_matrix) const
{
	// Parent matrix * bone matrix * bind matrix
	glm::mat4 bone_matrix = parent_matrix * mBones[index].matrix();
	matrices[index] = bone_matrix * mSkinMatrices[index];

	for (const int & child_index : mBones[index].mChildrenIndices)
		get_bone_matrices(matrices, child_index, bone_matrix);
}

void Skeleton::debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const
{
	static glm::vec4 color{ 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 bone_position = parent_matrix * glm::vec4{ mBones[index].mPosition, 1.0f };
	glm::mat4 next_parent_matrix = parent_matrix * mBones[index].matrix();

	Graphics & graphics = application.graphics();
	graphics.debug().draw_line(parent_position, bone_position, color);

	for (const auto & child_index : mBones[index].mChildrenIndices)
		debug_draw_rec(next_parent_matrix, bone_position, child_index);
}

const std::vector<glm::mat4> & Skeleton::skin_matrices() const { return mSkinMatrices; }
int Skeleton::bone_count() const { return mBones.size(); }
const Bone & Skeleton::root() const { return mBones.front(); }
