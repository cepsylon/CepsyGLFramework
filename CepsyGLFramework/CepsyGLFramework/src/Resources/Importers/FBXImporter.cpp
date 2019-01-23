#include "FBXImporter.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"
#include "Application/Application.h"
#include "Graphics/Model.h"

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
	application.resources().create<Model>("xbot", std::move(mMeshes), std::move(mMaterials));
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
	default:
		break;
	}
}

void FBXImporter::import_mesh(FbxNodeAttribute * attribute)
{
	// Load mesh
	FbxMesh * mesh = static_cast<FbxMesh *>(attribute);
	mMeshes.emplace_back(MeshImporter::load(mesh));

	// Check for materials and load
	FbxNode * material_node = mesh->GetNode();
	if (material_node)
	{
		for (int i = 0; i < material_node->GetMaterialCount(); ++i)
			mMaterials.emplace_back(MaterialImporter::load(material_node->GetMaterial(i)));
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
