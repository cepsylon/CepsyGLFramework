#include "SkeletonImporter.h"

#include "Resources/ResourceHandle.h"
#include "Graphics/Skeleton.h"
#include "Application/Application.h"

#include <fbxsdk/fbxsdk.h>

ResourceHandle<Skeleton> SkeletonImporter::load(const std::string & key, fbxsdk::FbxSkeleton * skeleton_node)
{
	// Check if already importer
	// TODO: check that all bones are named the same before not importing
	auto & skeleton_map = application.resources().get<Skeleton>();
	for (auto & pair : skeleton_map)
	{
		ResourceHandle<Skeleton> skeleton = static_cast<ResourceStorage<Skeleton> *>(pair.second.get());
		if (skeleton->root().mName == skeleton_node->GetNode()->GetName())
			return skeleton;
	}

	// Import skeleton
	Skeleton skeleton;
	import_bones_rec(skeleton_node, -1, skeleton);
	for (auto & matrix : skeleton.mSkinMatrices)
		matrix = glm::inverse(matrix);
	skeleton.set_name(key);
	return application.resources().create<Skeleton>(key, std::move(skeleton));
}

void SkeletonImporter::import_bones_rec(fbxsdk::FbxSkeleton * skeleton_node, int parent_index, Skeleton & skeleton)
{
	FbxNode * node = skeleton_node->GetNode();
	FbxDouble3 position = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	glm::quat quaternion = glm::vec3{ rotation[0], rotation[1], rotation[2] };
	glm::mat4 bind_matrix = glm::mat4_cast(quaternion);
	bind_matrix[3][0] = static_cast<float>(position[0]);
	bind_matrix[3][1] = static_cast<float>(position[1]);
	bind_matrix[3][2] = static_cast<float>(position[2]);

	// Add child to parent
	int next_parent_index = skeleton.mBones.size();
	if (parent_index != -1)
	{
		skeleton.mBones[parent_index].mChildrenIndices.emplace_back(next_parent_index);
		bind_matrix = skeleton.mSkinMatrices[parent_index] * bind_matrix;
	}

	// Add child
	skeleton.mBones.emplace_back(Bone{
		{}, node->GetName(),
		quaternion,
		glm::vec3{ position[0], position[1], position[2] },
		parent_index
		});
	skeleton.mSkinMatrices.emplace_back(bind_matrix);

	// Import children
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		for (int j = 0; j < node->GetNodeAttributeCount(); ++j)
		{
			FbxNodeAttribute * attribute = node->GetChild(i)->GetNodeAttributeByIndex(j);
			if (attribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				import_bones_rec(static_cast<FbxSkeleton *>(attribute), next_parent_index, skeleton);
				break;
			}
		}
	}
}
