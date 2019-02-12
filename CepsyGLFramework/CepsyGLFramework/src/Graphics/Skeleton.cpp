#include "Skeleton.h"

#include "Application/Application.h"

RTTI_I(Skeleton, Base);

Skeleton::Skeleton(std::vector<Bone> && bones)
	: mBones(std::move(bones))
{ }

bool Skeleton::empty() const { return mBones.empty(); }

void Skeleton::debug_draw() const
{

}

void Skeleton::debug_draw_rec(const glm::mat4 & parent_matrix, const glm::vec3 & parent_position, int index) const
{
	static glm::vec4 color{ 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 bone_position = parent_matrix * glm::vec4{ mBones[index].mPosition, 1.0f };

	Graphics & graphics = application.graphics();
	graphics.debug().draw_line(parent_position, glm::vec3{ bone_position.x, bone_position.y, bone_position.z }, color);
}
