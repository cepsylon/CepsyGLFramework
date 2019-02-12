#include "FBXImporter.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Application/Application.h"
#include "Graphics/Model.h"
#include "Graphics/SkeletalModel.h"

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

	// Free everything
	importer->Destroy();
	fbx_manager->Destroy();

	// Create the model
	if (mSkeleton.empty())
		application.resources().create<Model>("xbot", std::move(mMeshes), std::move(mMaterials));
	else
		application.resources().create<SkeletalModel>("xbot", std::move(mMeshes), std::move(mMaterials), std::move(mSkeleton));
}

void FBXImporter::import(FbxNode * node)
{
	FbxDouble3 position = node->LclTranslation.Get();
	FbxDouble3 scale = node->LclScaling.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	printf("\nName: %s\n", node->GetName());
	printf("Position: %f, %f, %f\nScale: %f, %f, %f\nRotation: %f, %f, %f\n", position[0], position[1], position[2], scale[0], scale[1], scale[2], rotation[0], rotation[1], rotation[2]);
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
	mMeshes.emplace_back(MeshImporter::load(mesh));
	for (int i = 0; i < mesh->GetDeformerCount(); ++i)
	{
		FbxDeformer * deformer = mesh->GetDeformer(i);
		switch (deformer->GetDeformerType())
		{
		case FbxDeformer::eSkin:
		{
			printf("Has skin\n");
			FbxSkin * skin = static_cast<FbxSkin *>(deformer);
			//skin->
			break;
		}
		case FbxDeformer::eBlendShape:
			printf("Has blend shape\n");
			break;
		case FbxDeformer::eVertexCache:
			printf("Has vertex cache\n");
			break;
		default:
			break;
		}
	}

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
	Skeleton test{ std::move(bones) };
}

void FBXImporter::import_bones_rec(FbxSkeleton * skeleton_node, int parent_index, std::vector<Skeleton::Bone> & bones)
{
	// Add child to parent
	int next_parent_index = bones.size();
	if (parent_index != -1)
		bones[parent_index].mChildrenIndices.emplace_back(next_parent_index);

	// Add child
	FbxNode * node = skeleton_node->GetNode();
	FbxDouble3 position = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	bones.emplace_back(Skeleton::Bone{ {}, node->GetName(),
		glm::vec3{ position[0], position[1], position[2] },
		glm::vec3{ rotation[0], rotation[1], rotation[2] }
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

#ifdef _DEBUG
void FBXImporter::print_scene(FbxScene * scene)
{
	FbxNode * root = scene->GetRootNode();
	if (root)
	{
		// Print everything
		for (int i = 0; i < root->GetChildCount(); ++i)
			print_node(root->GetChild(i), 0);
	}
}

void FBXImporter::print_node(FbxNode * node, int tab_count)
{
	// Print node name
	print_tabs(tab_count);
	const char * name = node->GetName();
	printf("<node name='%s'>\n", name);

	int tabs = tab_count + 1;
	// Print node attributes
	for (int i = 0; i < node->GetNodeAttributeCount(); ++i)
		print_attribute(node->GetNodeAttributeByIndex(i), tabs);

	// Print node children
	for (int i = 0; i < node->GetChildCount(); ++i)
		print_node(node->GetChild(i), tabs);

	print_tabs(tab_count);
	printf("</node>\n");
}

void FBXImporter::print_attribute(FbxNodeAttribute * attribute, int tab_count)
{
	if (!attribute)
		return;

	std::string type = attribute_type(attribute->GetAttributeType());
	print_tabs(tab_count);
	printf("<attribute type='%s' name='%s'>\n", type.c_str(), attribute->GetName());
}


std::string FBXImporter::attribute_type(FbxNodeAttribute::EType type)
{
	switch (type)
	{
	case FbxNodeAttribute::eUnknown: return "unidentified";
	case FbxNodeAttribute::eNull: return "null";
	case FbxNodeAttribute::eMarker: return "marker";
	case FbxNodeAttribute::eSkeleton: return "skeleton";
	case FbxNodeAttribute::eMesh: return "mesh";
	case FbxNodeAttribute::eNurbs: return "nurbs";
	case FbxNodeAttribute::ePatch: return "patch";
	case FbxNodeAttribute::eCamera: return "camera";
	case FbxNodeAttribute::eCameraStereo: return "stereo";
	case FbxNodeAttribute::eCameraSwitcher: return "camera switcher";
	case FbxNodeAttribute::eLight: return "light";
	case FbxNodeAttribute::eOpticalReference: return "optical reference";
	case FbxNodeAttribute::eOpticalMarker: return "marker";
	case FbxNodeAttribute::eNurbsCurve: return "nurbs curve";
	case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface";
	case FbxNodeAttribute::eBoundary: return "boundary";
	case FbxNodeAttribute::eNurbsSurface: return "nurbs surface";
	case FbxNodeAttribute::eShape: return "shape";
	case FbxNodeAttribute::eLODGroup: return "lodgroup";
	case FbxNodeAttribute::eSubDiv: return "subdiv";
	default: return "unknown";
	}
}

void FBXImporter::print_tabs(int count)
{
	for (int i = 0; i < count; ++i)
		printf("\t");
}
#endif
