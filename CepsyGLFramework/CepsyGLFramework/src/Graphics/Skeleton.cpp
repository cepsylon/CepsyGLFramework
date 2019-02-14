#include "Skeleton.h"

#include "Application/Application.h"

RTTI_I(Skeleton, Base);

glm::mat4 Skeleton::Bone::matrix() const
{
	glm::mat4 matrix = glm::mat4_cast(mRotation);
	matrix[3][0] = mPosition.x;
	matrix[3][1] = mPosition.y;
	matrix[3][2] = mPosition.z;
	return matrix;
}

Skeleton::Skeleton(std::vector<Bone> && bones)
	: mBones(std::move(bones))
{
	mBuffer.generate();
}

Skeleton::Skeleton(Skeleton && rhs)
	: mBones(std::move(rhs.mBones))
	, mBuffer(std::move(rhs.mBuffer))
{ }

Skeleton & Skeleton::operator=(Skeleton && rhs)
{
	if (this != &rhs)
	{
		mBones = std::move(rhs.mBones);
		mBuffer = std::move(rhs.mBuffer);
	}

	return *this;
}

bool Skeleton::empty() const { return mBones.empty(); }

void Skeleton::update()
{
	// Update buffer
	std::vector<glm::mat4> bone_matrices = get_bone_matrices();
	mBuffer.update(bone_matrices.data(), bone_matrices.size() * sizeof(glm::mat4));
}

void Skeleton::bind() const
{
	mBuffer.bind_base(1);
}

void Skeleton::debug_draw(const glm::mat4 & model_matrix) const
{
	glm::vec4 bone_position = model_matrix * glm::vec4{ mBones.front().mPosition, 1.0f };
	glm::mat4 child_matrix = model_matrix * mBones.front().matrix();
	for (const auto & child_index : mBones.front().mChildrenIndices)
		debug_draw_rec(child_matrix, bone_position, child_index);
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
	bone_matrices.front() = parent_matrix * mBones.front().mBindMatrix;

	// Call recursion on children
	for (const int & child_index : mBones.front().mChildrenIndices)
		get_bone_matrices(bone_matrices, child_index, parent_matrix);

	return bone_matrices;
}

void Skeleton::get_bone_matrices(std::vector<glm::mat4> & matrices, int index, const glm::mat4 & parent_matrix) const
{
	// Parent matrix * bone matrix * bind matrix
	glm::mat4 bone_matrix = parent_matrix * mBones[index].matrix();
	matrices[index] = bone_matrix * mBones[index].mBindMatrix;

	for (const int & child_index : mBones[index].mChildrenIndices)
		get_bone_matrices(matrices, child_index, bone_matrix);
}

void Skeleton::debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const
{
	static glm::vec4 color{ 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 bone_position = parent_matrix * glm::vec4{ mBones[index].mPosition, 1.0f };
	glm::mat4 child_matrix = parent_matrix * mBones[index].matrix();

	Graphics & graphics = application.graphics();
	graphics.debug().draw_line(parent_position, bone_position, color);

	for (const auto & child_index : mBones[index].mChildrenIndices)
		debug_draw_rec(child_matrix, bone_position, child_index);
}

int Skeleton::bone_count() const { return mBones.size(); }
