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
{ }

bool Skeleton::empty() const { return mBones.empty(); }

void Skeleton::debug_draw(const glm::mat4 & model_matrix) const
{
	glm::vec4 bone_position = model_matrix * glm::vec4{ mBones.front().mPosition, 1.0f };
	glm::mat4 child_matrix = model_matrix * mBones.front().matrix();
	for (const auto & child_index : mBones.front().mChildrenIndices)
		debug_draw_rec(child_matrix, bone_position, child_index);
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
