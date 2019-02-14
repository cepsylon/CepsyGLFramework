#include "FBXImporter.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "AnimationImporter.h"
#include "Application/Application.h"
#include "Graphics/Model.h"
#include "Graphics/SkeletalModel.h"

#include <fbxsdk/core/fbxpropertydef.h>
#include <fbxsdk/fbxsdk.h>
#include <glm/vec3.hpp>

#include <vector>

void FBXImporter::load(const std::string & path)
{
	// Create new SDK manager
	FbxManager * fbx_manager = FbxManager::Create();

	// Create input settings
	FbxIOSettings * io_settings = FbxIOSettings::Create(fbx_manager, IOSROOT);
	fbx_manager->SetIOSettings(io_settings);

	// Create importer with the path as a name, this does not load the file
	FbxImporter * importer = FbxImporter::Create(fbx_manager, path.c_str());

	// Initialize importer
	if (!importer->Initialize(path.c_str(), -1, fbx_manager->GetIOSettings()))
	{
		printf("Error loading file %s\n", path.c_str());
		return;
	}

	// Create scene and import it
	FbxScene * scene = FbxScene::Create(fbx_manager, "Scene");
	importer->Import(scene);

	// Import all nodes
	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		for (int i = 0; i < root->GetChildCount(); ++i)
			import(root->GetChild(i));
	}

	// Animation importing
	//for (int i = 0; i < scene->GetSrcObjectCount<FbxAnimStack>(); ++i)
	//{
	//	FbxAnimStack * stack = scene->GetSrcObject<FbxAnimStack>(i);
	//	printf("Animation stack name: %s\n", stack->GetName());
	//
	//	// Animation layer
	//	for (int j = 0; j < stack->GetMemberCount<FbxAnimLayer>(); ++j)
	//	{
	//		FbxAnimLayer * layer = stack->GetMember<FbxAnimLayer>(j);
	//		printf("\tAnimation layer name: %s\n", layer->GetName());
	//	}
	//}
	//DisplayAnimation(scene);
	AnimationImporter anim_importer(mSkeleton);
	anim_importer.load(scene);

	// Free everything
	importer->Destroy();
	fbx_manager->Destroy();

	// Create the model
	unsigned start = path.find_last_of("/") + 1;
	unsigned end = path.find_last_of(".");
	std::string name = path.substr(start, end - start);
	if (mSkeleton.empty())
		application.resources().create<Model>(name, std::move(mMeshes), std::move(mMaterials));
	else
		application.resources().create<SkeletalModel>(name, std::move(mMeshes), std::move(mMaterials), std::move(mSkeleton));
}

void FBXImporter::import(FbxNode * node)
{
	// Import all attributes to the node
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		import(node->GetNodeAttributeByIndex(i));

	// Import all children
	for (int i = 0; i < node->GetChildCount(); ++i)
		import(node->GetChild(i));
}

void FBXImporter::import(FbxNodeAttribute * attribute)
{
	if (!attribute)
		return;

	// Check the type and import depending on that
	// For now, we will only import meshes
	switch (attribute->GetAttributeType())
	{
	case FbxNodeAttribute::eMesh:
		import_mesh(attribute);
		break;
	case FbxNodeAttribute::eSkeleton:
	{
		FbxSkeleton * skeleton = static_cast<FbxSkeleton *>(attribute);
		if(skeleton->IsSkeletonRoot())
			import_skeleton(skeleton);
	}
		break;
	default:
		break;
	}
}

void FBXImporter::import_mesh(FbxNodeAttribute * attribute)
{
	// Load mesh
	FbxMesh * mesh = static_cast<FbxMesh *>(attribute);
	mMeshes.emplace_back(MeshImporter::load(mesh, mSkeleton));

	// Check for materials and load
	FbxNode * material_node = mesh->GetNode();
	if (material_node)
	{
		for (int i = 0; i < material_node->GetMaterialCount(); ++i)
			mMaterials.emplace_back(MaterialImporter::load(material_node->GetMaterial(i)));
	}
}

void FBXImporter::import_skeleton(FbxSkeleton * skeleton)
{
	std::vector<Skeleton::Bone> bones;
	import_bones_rec(skeleton, -1, bones);
	for (auto & bone : bones)
		bone.mBindMatrix = glm::inverse(bone.mBindMatrix);
	mSkeleton = Skeleton{ std::move(bones) };
}

void FBXImporter::import_bones_rec(FbxSkeleton * skeleton_node, int parent_index, std::vector<Skeleton::Bone> & bones)
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
	int next_parent_index = bones.size();
	if (parent_index != -1)
	{
		bones[parent_index].mChildrenIndices.emplace_back(next_parent_index);
		bind_matrix = bones[parent_index].mBindMatrix * bind_matrix;
	}

	// Add child
	bones.emplace_back(Skeleton::Bone{ bind_matrix, {}, node->GetName(),
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
