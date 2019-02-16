#include "FBXImporter.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "SkeletonImporter.h"
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
	AnimationImporter anim_importer(*mSkeleton);
	anim_importer.load(scene);

	// Free everything
	importer->Destroy();
	fbx_manager->Destroy();

	// Create the model
	unsigned start = path.find_last_of("/") + 1;
	unsigned end = path.find_last_of(".");
	std::string name = path.substr(start, end - start);
	if ((*mSkeleton).empty())
		application.resources().create<Model>(name, std::move(mMeshes), std::move(mMaterials));
	else
		application.resources().create<SkeletalModel>(name, std::move(mMeshes), std::move(mMaterials), mSkeleton);
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
		if (skeleton->IsSkeletonRoot())
			mSkeleton = SkeletonImporter::load("xbot", skeleton);
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
	mMeshes.emplace_back(MeshImporter::load(mesh, *mSkeleton));

	// Check for materials and load
	FbxNode * material_node = mesh->GetNode();
	if (material_node)
	{
		for (int i = 0; i < material_node->GetMaterialCount(); ++i)
			mMaterials.emplace_back(MaterialImporter::load(material_node->GetMaterial(i)));
	}
}
