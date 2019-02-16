#include "SkeletonImporter.h"

#include "Graphics/Skeleton.h"
#include "Application/Application.h"

#include <fbxsdk/fbxsdk.h>

Skeleton * SkeletonImporter::load(const std::string & key, fbxsdk::FbxSkeleton * skeleton)
{
	std::vector<Bone> bones;
	import_bones_rec(skeleton, -1, bones);
	for (auto & bone : bones)
		bone.mBindMatrix = glm::inverse(bone.mBindMatrix);
	return application.resources().create<Skeleton>(key, std::move(bones));
}

void SkeletonImporter::import_bones_rec(fbxsdk::FbxSkeleton * skeleton, int parent_index, std::vector<Bone> & bones)
{
	FbxNode * node = skeleton->GetNode();
	FbxDouble3 position = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	glm::quat quaternion = glm::vec3{ rotation[0], rotation[1], rotation[2] };
	glm::mat4 bind_matrix = glm::mat4_cast(quaternion);
	bind_matrix[3][0] = static_cast<float>(position[0]);
	bind_matrix[3][1] = static_cast<float>(position[1]);
	bind_matrix[3][2] = static_cast<float>(position[2]);

	// Add child to parent
	int next_parent_index = bones.size();
	if (parent_index != -1)
	{
		bones[parent_index].mChildrenIndices.emplace_back(next_parent_index);
		bind_matrix = bones[parent_index].mBindMatrix * bind_matrix;
	}

	// Add child
	bones.emplace_back(Bone{ bind_matrix, {}, node->GetName(),
		quaternion,
		glm::vec3{ position[0], position[1], position[2] }
		});

	// Import children
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		for (int j = 0; j < node->GetNodeAttributeCount(); ++j)
		{
			FbxNodeAttribute * attribute = node->GetChild(i)->GetNodeAttributeByIndex(j);
			if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				import_bones_rec(static_cast<FbxSkeleton *>(attribute), next_parent_index, bones);
				break;
			}
		}
	}
}
